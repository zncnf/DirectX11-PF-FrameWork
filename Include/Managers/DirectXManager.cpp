#include "D3DUtil.h"
#include "DirectXManager.h"

DirectXManager * DirectXManager::instance = nullptr;

DirectXManager::~DirectXManager()
{
	ReleaseCOM(pDepthStencilBuffer);
	ReleaseCOM(pDepthStencilState);
	ReleaseCOM(pDepthdStencilState_2D);
	ReleaseCOM(pDepthStencilView);
	ReleaseCOM(pRender_Target_View);
	ReleaseCOM(pRasterState);
	ReleaseCOM(pDevice);
	ReleaseCOM(pDevice_Context);
	ReleaseCOM(pSwap_Chain);
	ReleaseCOM(pRT);
	ReleaseCOM(pD2DFactory);
	ReleaseCOM(pDWriteFactory);
	ReleaseCOM(pAlphaBlendingState);
	ReleaseCOM(pD2DFactory);
	ReleaseCOM(pDWriteFactory);
	ReleaseCOM(pRT);
}

DirectXManager * DirectXManager::GetInstance()
{
	if (!instance)
		instance = new DirectXManager;

	return instance;
}

void DirectXManager::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void DirectXManager::Init()
{
	if (pSwap_Chain)
		pSwap_Chain->Release();

	width  = static_cast<UINT>(WindowManager::GetInstance()->GetWindowWidth());
	height = static_cast<UINT>(WindowManager::GetInstance()->GetWindowHeight());

	CreateDeviceAndSwapChain();
	CreateRanderTargetView();
	CreateDepthBuffer();
	CreateStencilBuffer();
	CreateDepthStencilView();
	CreateDepthStencilView_2D();
	CreateRasterizer();
	CreateViewPort();
	CreateBlendState();
	CreateDirect2D();
}

#pragma region InitDirect3D
void DirectXManager::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = WindowManager::GetInstance()->GetWindowHandle();
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//기능 지원 점검
	vector<D3D_FEATURE_LEVEL> feature
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, // Direct3D Resource와 Direct2D 상호 운용성을 위해 필요한 셋팅
		feature.data(),
		(UINT)feature.size(),
		D3D11_SDK_VERSION,
		&desc,
		&pSwap_Chain,
		&pDevice,
		nullptr,
		&pDevice_Context);
}

void DirectXManager::CreateRanderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;

	pSwap_Chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	pSwap_Chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	pDevice->CreateRenderTargetView(backBuffer, nullptr, &pRender_Target_View);

	ReleaseCOM(backBuffer);
}

void DirectXManager::CreateDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &pDepthStencilBuffer);
}

void DirectXManager::CreateStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Stencil Struct
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil Front
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil Back
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	pDevice_Context->OMSetDepthStencilState(pDepthStencilState, 1);
}

void DirectXManager::CreateDepthStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);
	pDevice_Context->OMSetRenderTargets(1, &pRender_Target_View, pDepthStencilView);
}

void DirectXManager::CreateDepthStencilView_2D()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc_2D;
	ZeroMemory(&depthStencilDesc_2D, sizeof(depthStencilDesc_2D));

	depthStencilDesc_2D.DepthEnable = false;
	depthStencilDesc_2D.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc_2D.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc_2D.StencilEnable = true;
	depthStencilDesc_2D.StencilReadMask = 0xFF;
	depthStencilDesc_2D.StencilWriteMask = 0xFF;
	depthStencilDesc_2D.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc_2D.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc_2D.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc_2D.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc_2D.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc_2D.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc_2D.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc_2D.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	pDevice->CreateDepthStencilState(&depthStencilDesc_2D, &pDepthdStencilState_2D);
}

void DirectXManager::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
	pDevice_Context->RSSetState(pRasterState);
}

void DirectXManager::CreateViewPort()
{
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = static_cast<float>(width);
	viewPort.Height = static_cast<float>(height);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	pDevice_Context->RSSetViewports(1, &viewPort);
}

void DirectXManager::CreateBlendState()
{
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	pDevice->CreateBlendState(&blendStateDescription, &pAlphaBlendingState);
	TurnOnAlphaBlending();
}
#pragma endregion

#pragma region InitDirect2D
void DirectXManager::CreateDirect2D()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

	FLOAT dpiX;
	FLOAT dpiY;
	pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY);

	IDXGISurface* pBackBuffer;
	pSwap_Chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer, &props, &pRT);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	pSwap_Chain->GetDesc(&swapDesc);
}
#pragma endregion

#pragma region Options
void DirectXManager::TurnOnZBuffer()
{
	pDevice_Context->OMSetDepthStencilState(pDepthStencilState, 1);
}

void DirectXManager::TurnOffZBuffer()
{
	pDevice_Context->OMSetDepthStencilState(pDepthdStencilState_2D, 1);
}

void DirectXManager::TurnOnAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	pDevice_Context->OMSetBlendState(pAlphaBlendingState, blendFactor, 0xffffffff);
}

void DirectXManager::BeginDraw()
{
	pDevice_Context->ClearRenderTargetView(pRender_Target_View, clearColor);
	pDevice_Context->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectXManager::EndDraww()
{
	pSwap_Chain->Present(1, 0);
}

ID2D1RenderTarget* DirectXManager::GetRenderTarget()
{
	return pRT;
}
IDWriteFactory * DirectXManager::GetWriteFactory()
{
	return pDWriteFactory;
}
#pragma endregion



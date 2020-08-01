#pragma once
class DirectXManager final
{
private:
	DirectXManager() = default;
public:
	~DirectXManager();

public: 
	static DirectXManager* instance;

	static DirectXManager* GetInstance();
	void   ReleaseInstance();

public:
	void Init();

	void TurnOnZBuffer();
	void TurnOffZBuffer();
	void TurnOnAlphaBlending();

	void BeginDraw();
	void EndDraww();

	ID2D1RenderTarget* GetRenderTarget();
	IDWriteFactory* GetWriteFactory();

private: // Direct3D
	ID3D11Device*            pDevice                    = nullptr;
	ID3D11DeviceContext*     pDevice_Context            = nullptr;
	IDXGISwapChain *         pSwap_Chain                = nullptr;
	ID3D11RenderTargetView*  pRender_Target_View        = nullptr;
	ID3D11Texture2D*         pDepthStencilBuffer        = nullptr;
	ID3D11DepthStencilState* pDepthStencilState         = nullptr;
	ID3D11DepthStencilView*  pDepthStencilView          = nullptr;
	ID3D11RasterizerState*   pRasterState               = nullptr;
	ID3D11DepthStencilState* pDepthdStencilState_2D     = nullptr;
	ID3D11BlendState*        pAlphaBlendingState        = nullptr;


	D3D11_VIEWPORT viewPort   = { 0 };
	D3DXCOLOR      clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	UINT width;
	UINT height;

	void CreateDeviceAndSwapChain();
	void CreateRanderTargetView();
	void CreateDepthBuffer();
	void CreateStencilBuffer();
	void CreateDepthStencilView();
	void CreateDepthStencilView_2D();
	void CreateRasterizer();
	void CreateViewPort();
	void CreateBlendState();

private: // Direct2D, DirectWrite
	ID2D1Factory*		pD2DFactory    = nullptr;
	IDWriteFactory*		pDWriteFactory = nullptr;
	ID2D1RenderTarget*  pRT            = nullptr;

	void CreateDirect2D();
};


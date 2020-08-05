#include "D3DUtil.h"
#include "DefaultShader_Specular.h"

DefaultShader_Specular::~DefaultShader_Specular()
{
	ReleaseCOM(VS_Shader);
	ReleaseCOM(PS_Shader);
	ReleaseCOM(inputLayout);
	ReleaseCOM(matrixBuffer);
	ReleaseCOM(lightBuffer);
	ReleaseCOM(cameraBuffer);
}

void DefaultShader_Specular::Init()
{
	ID3DBlob* vertexShaderBuffer = nullptr;
	ID3DBlob* pixelShaderBuffer  = nullptr;

	D3DX11CompileFromFileA("Resource/Shaders/DefaultShader_Specular.hlsl", NULL, NULL, "LightVertexShader", "vs_5_0", 0, 0, NULL, &vertexShaderBuffer, NULL, NULL);
	D3DX11CompileFromFileA("Resource/Shaders/DefaultShader_Specular.hlsl", NULL, NULL, "LightPixelShader", "ps_5_0", 0, 0, NULL, &pixelShaderBuffer, NULL, NULL);

	DirectXManager::GetInstance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &VS_Shader);
	DirectXManager::GetInstance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &PS_Shader);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth           = sizeof(CBuffer_Matrix);
	matrixBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags           = 0;
	matrixBufferDesc.StructureByteStride = 0;

	DirectXManager::GetInstance()->GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth			 = sizeof(CBuffer_Camera);
	cameraBufferDesc.BindFlags			 = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags		 = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags			 = 0;
	cameraBufferDesc.StructureByteStride = 0;

	DirectXManager::GetInstance()->GetDevice()->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);

	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage			    = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth			= sizeof(ShaderType_Specular);
	lightBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags	    = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags			= 0;
	lightBufferDesc.StructureByteStride = 0;

	DirectXManager::GetInstance()->GetDevice()->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	D3D11_INPUT_ELEMENT_DESC input_desc[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DirectXManager::GetInstance()->GetDevice()->CreateInputLayout(input_desc, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	ReleaseCOM(vertexShaderBuffer);
	ReleaseCOM(pixelShaderBuffer);
}

void DefaultShader_Specular::Update(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR4 diffuseColor, D3DXVECTOR4 ambientColor, D3DXVECTOR4 specularColor, float specularPower)
{
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	#pragma region SetCbuffer_Matrix
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(DirectXManager::GetInstance()->GetDeviceContext()->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return;

	CBuffer_Matrix* dataPtr = (CBuffer_Matrix*)mappedResource.pData;
	dataPtr->m_worldMatrix = world;
	dataPtr->m_viewMatrix = view;
	dataPtr->m_projectionMatrix = projection;

	DirectXManager::GetInstance()->GetDeviceContext()->Unmap(matrixBuffer, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 1, &matrixBuffer);
	#pragma endregion

	#pragma region SetCbuffer_Specular
	D3D11_MAPPED_SUBRESOURCE mappedResource2;
	if (FAILED(DirectXManager::GetInstance()->GetDeviceContext()->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2)))
		return;

	ShaderType_Specular* dataPtr2 = (ShaderType_Specular*)mappedResource2.pData;
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = GameManager::GetInstance()->drectionalLight;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;

	DirectXManager::GetInstance()->GetDeviceContext()->Unmap(lightBuffer, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(0, 1, &lightBuffer);
	#pragma endregion

	#pragma region SetBuffer_Camera
	D3D11_MAPPED_SUBRESOURCE mappedResource3;
	if (FAILED(DirectXManager::GetInstance()->GetDeviceContext()->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource3)))
		return;

	CBuffer_Camera* dataPtr3 = (CBuffer_Camera*)mappedResource3.pData;
	dataPtr3->cameraPosition = GameManager::GetInstance()->camera_Position;
	dataPtr3->padding = 0.0f;

	DirectXManager::GetInstance()->GetDeviceContext()->Unmap(cameraBuffer, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(1, 1, &cameraBuffer);
	#pragma endregion

	#pragma region Render
	DirectXManager::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXManager::GetInstance()->GetDeviceContext()->IASetInputLayout(inputLayout);

	//VertexShader
	DirectXManager::GetInstance()->GetDeviceContext()->VSSetShader(VS_Shader, NULL, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 1, &matrixBuffer);
	DirectXManager::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(1, 1, &cameraBuffer);

	//PixelShader
	DirectXManager::GetInstance()->GetDeviceContext()->PSSetShader(PS_Shader, NULL, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(0, 1, &lightBuffer);
	#pragma endregion
}

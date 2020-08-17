#pragma once
#include "Shader.h"
class DefaultShader_Specular : public Shader
{
public:
	DefaultShader_Specular() = default;
	virtual ~DefaultShader_Specular();

public:
	virtual void Init() override;
	virtual void Update(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR4 diffuseColor, D3DXVECTOR4 ambientColor, D3DXVECTOR4 specularColor, float specularPower, std::vector<struct BoneInfo>* boneInfo = nullptr) override;

private:
	ID3D11VertexShader* VS_Shader = nullptr;
	ID3D11PixelShader*  PS_Shader = nullptr;

	ID3D11InputLayout* inputLayout  = nullptr;
	ID3D11Buffer*      matrixBuffer = nullptr;
	ID3D11Buffer*      lightBuffer  = nullptr;
	ID3D11Buffer*      cameraBuffer = nullptr;

	ID3D11ShaderResourceView* shaderResource = nullptr;
	ID3D11SamplerState* 	  sampleState    = nullptr;
};


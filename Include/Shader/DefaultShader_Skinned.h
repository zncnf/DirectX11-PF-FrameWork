#pragma once
#include "Shader/Shader.h"

class DefaultShader_Skinned : public Shader
{
public:
	DefaultShader_Skinned() = default;
	virtual ~DefaultShader_Skinned();

public:
	virtual void Init() override;
	virtual void Update(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR4 diffuseColor, D3DXVECTOR4 ambientColor, D3DXVECTOR4 specularColor, float specularPower, std::vector<struct BoneInfo>* boneInfo = nullptr) override;

private:
	ID3D11VertexShader* VS_Shader = nullptr;
	ID3D11PixelShader*  PS_Shader = nullptr;

	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer*      matrixBuffer = nullptr;
	ID3D11Buffer*      boneBuffer = nullptr;
	ID3D11Buffer*      lightBuffer = nullptr;
	ID3D11Buffer*      cameraBuffer = nullptr;
};


#pragma once
class Shader
{
public:
	Shader() = default;
	~Shader() = default;
public:
	virtual void Init() = 0;
	virtual void Update(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR4 diffuseColor, D3DXVECTOR4 ambientColor, D3DXVECTOR4 specularColor, float specularPower, std::vector<struct BoneInfo>* boneInfo = nullptr);
};


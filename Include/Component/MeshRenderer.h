#pragma once
#include "Component.h"
#include "Shader/Shader.h"
class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* _object, const aiScene* _pScene);
	virtual ~MeshRenderer();
public:
	void Init() override;
	void Update() override;
public:
	void setShader(std::shared_ptr<Shader> _pShader);
	std::shared_ptr<Shader> getShader();
public:
	std::shared_ptr<Shader> pShader;
private:
	const aiScene* pScene = nullptr;

	aiColor3D diffuseColor;
	aiColor4D ambientColor;
	aiColor4D specularColor;

	ID3D11ShaderResourceView* m_ShaderResource = nullptr;
	ID3D11SamplerState*	m_SampleState = nullptr;
};


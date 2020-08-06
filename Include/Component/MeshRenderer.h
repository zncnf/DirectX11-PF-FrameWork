#pragma once
#include "Structure/Component.h"
#include "Shader/Shader.h"
class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* _object, const aiScene* _pScene, aiNode* _node);
	virtual ~MeshRenderer();
public:
	void Init() override;
	void Update() override;

public:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessMaterial(aiMaterial* mat, const aiScene* scene);

public:
	void setShader(std::shared_ptr<Shader> _pShader);
	std::shared_ptr<Shader> getShader();
public:
	std::shared_ptr<Shader> pShader;
private:
	const aiScene* pScene = nullptr;
	aiNode* node = nullptr;

	aiColor3D diffuseColor;
	aiColor4D ambientColor;
	aiColor4D specularColor;

	ID3D11ShaderResourceView* m_ShaderResource = nullptr;
	ID3D11SamplerState*	m_SampleState = nullptr;
};


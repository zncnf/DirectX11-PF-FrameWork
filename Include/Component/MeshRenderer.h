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
	void SetShader(std::shared_ptr<Shader> _pShader);
	void SetTexturePath(string path);
	std::shared_ptr<Shader> GetShader();
public:
	std::shared_ptr<Shader> pShader;

	aiColor4D diffuseColor;
	aiColor4D ambientColor;
	aiColor4D specularColor;

	aiMaterial* pMaterial;
private:
	const aiScene* pScene = nullptr;
	aiNode* node = nullptr;

	ID3D11ShaderResourceView* m_ShaderResource = nullptr;
	ID3D11SamplerState*	m_SampleState = nullptr;
};


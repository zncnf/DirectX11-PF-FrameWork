#pragma once
#include "Structure/Component.h"

class SkinnedMeshRenderer : public Component
{
public:
	SkinnedMeshRenderer(GameObject* _object, const aiScene* _pScene, aiNode* _node);
	virtual ~SkinnedMeshRenderer();

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessBone(aiMesh* mesh, const aiScene* scene);
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
	aiMesh*  pMesh = nullptr;
	const aiFace* pFace = nullptr;
	aiNode* node = nullptr;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	UINT indexCount;
	UINT vertexCount;
	UINT boneCount;

	std::vector<VertexType_SkindMesh> _vertices;
	std::vector<VertexBoneData> _bones;
	std::vector<DWORD> _indices;

	std::vector<SkinDrawData> skinDrawData;
	std::vector<BoneInfo> boneInfo;
	std::map<std::string, UINT> boneMapping;

	VertexType_SkindMesh* vertices;
	UINT* indices;

	UINT indexoffset;

	UINT offset;
private:
	ID3D11ShaderResourceView* m_ShaderResource = nullptr;
	ID3D11SamplerState*	m_SampleState = nullptr;
};


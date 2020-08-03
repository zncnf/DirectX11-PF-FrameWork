#pragma once
#include "Component.h"
class MeshFilter : public Component
{
public:
	MeshFilter(GameObject* _object, const aiScene* _pScene, aiNode* _node);
	virtual ~MeshFilter();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	const aiScene* pScene = nullptr;
	const aiMesh* pMesh   = nullptr;
	const aiFace* pFace   = nullptr;
	aiNode* node		  = nullptr;
	aiMaterial* pMaterial = nullptr;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer  = nullptr;

	UINT indexCount;
	UINT vertexCount;

	std::vector<VertexType_PTN> _vertices;
	std::vector<DWORD> _indices;

	VertexType_PTN* vertices;
	UINT* indices;

	UINT offset;
};


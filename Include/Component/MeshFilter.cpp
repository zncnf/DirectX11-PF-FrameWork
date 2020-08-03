#include "D3DUtil.h"
#include "MeshFilter.h"

MeshFilter::MeshFilter(GameObject * _object, const aiScene * _pScene, aiNode * _node)
{
	gameObject = _object;
	pScene	   = _pScene;
	node       = _node;

	vertexCount = 0;
	indexCount	= 0;
	offset		= 0;
}

MeshFilter::~MeshFilter()
{
	ReleaseCOM(vertexBuffer);
	ReleaseCOM(indexBuffer);
}

void MeshFilter::Init()
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		vertexCount += pScene->mMeshes[node->mMeshes[i]]->mNumVertices;
		indexCount  += pScene->mMeshes[node->mMeshes[i]]->mNumFaces * 3;
	}

	vertices = new VertexType_PTN[vertexCount];
	indices  = new UINT[indexCount];

	ProcessNode(node, pScene);

	for (int i = 0; i < _vertices.size(); i++)
	{
		vertices[i] = _vertices[i];
	}

	for (int i = 0; i < _indices.size(); i++)
	{
		indices[i] = _indices[i];
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType_PTN) *vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(DirectXManager::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
		return;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(DirectXManager::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
		return;

	delete[] vertices;
	delete[] indices;
	vertices = 0;
	indices = 0;
}

void MeshFilter::Update()
{
	UINT stride = sizeof(VertexType_PTN);
	UINT offset = 0;

	DirectXManager::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DirectXManager::GetInstance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXManager::GetInstance()->GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void MeshFilter::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
}

void MeshFilter::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VertexType_PTN vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texture.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texture.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		_vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			_indices.push_back(face.mIndices[j] + offset);
		}
	}

	offset += mesh->mNumVertices;
}

#include "D3DUtil.h"
#include "SkinnedMeshRenderer.h"
#include "Transform.h"

SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject * _object, const aiScene * _pScene, aiNode * _node)
{
	gameObject = _object;
	pScene     = _pScene;
	node       = _node;

	indexoffset = 0;
	boneCount = 0;

	name = _node->mName.data;

	pShader = ResourceManager::GetInstance()->GetShader("DefaultShader_Skinned");
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	ReleaseCOM(vertexBuffer);
	ReleaseCOM(indexBuffer);

	pShader.reset();
}

void SkinnedMeshRenderer::Init()
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		vertexCount += pScene->mMeshes[node->mMeshes[i]]->mNumVertices;
		indexCount += pScene->mMeshes[node->mMeshes[i]]->mNumFaces * 3;
	}

	_bones.resize(vertexCount);

	ProcessNode(node, pScene);

	vertices = new VertexType_SkindMesh[vertexCount];
	indices = new UINT[indexCount];

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
	vertexBufferDesc.ByteWidth = sizeof(VertexType_SkindMesh) * vertexCount;
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

	if (pShader)
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.MaxAnisotropy = 16;
		desc.MaxLOD = std::numeric_limits<float>::max();
		desc.MinLOD = std::numeric_limits<float>::min();
		desc.MipLODBias = 0.0f;

		DirectXManager::GetInstance()->GetDevice()->CreateSamplerState(&desc, &m_SampleState);
	}
}

void SkinnedMeshRenderer::Update()
{
	if (pShader)
	{
		D3DXVECTOR4 diffuse(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0f);
		D3DXVECTOR4 ambient(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
		D3DXVECTOR4 specular(specularColor.r, specularColor.g, specularColor.b, specularColor.a);

		pShader->Update(gameObject->transform->worldMatrix, GameManager::GetInstance()->viewMatrix, GameManager::GetInstance()->projectionMatrix,
			diffuse, ambient, specular, 32.0f, &boneInfo, _indices.size());

		DirectXManager::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_ShaderResource);
		DirectXManager::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_SampleState);
	}

	UINT stride = sizeof(VertexType_SkindMesh);
	UINT offset = 0;

	DirectXManager::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DirectXManager::GetInstance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DirectXManager::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXManager::GetInstance()->GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void SkinnedMeshRenderer::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		if (ResourceManager::GetInstance()->GetMesh(scene->mMeshes[node->mMeshes[i]]->mName.C_Str()))
		{
			pMesh = ResourceManager::GetInstance()->GetMesh(scene->mMeshes[node->mMeshes[i]]->mName.C_Str());
		}
		else
		{
			pMesh = scene->mMeshes[node->mMeshes[i]];
			ResourceManager::GetInstance()->AddMesh(pMesh->mName.C_Str(), pMesh);
		}

		ProcessMesh(pMesh, scene);
	}
}

void SkinnedMeshRenderer::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	ProcessBone(mesh, scene);

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VertexType_SkindMesh vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texture.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texture.y = (float)mesh->mTextureCoords[0][i].y;
		}

		for (int j = 0; j < 4; ++j)
		{
			vertex.boneID[j] = _bones[i].Ids[j];
			vertex.weights[j] = _bones[i].Weights[j];
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

	if (mesh->mMaterialIndex >= 0)
	{
		aiString matName;

		pMaterial = scene->mMaterials[mesh->mMaterialIndex];
		pMaterial->Get(AI_MATKEY_NAME, matName);

		if (ResourceManager::GetInstance()->GetMaterial(matName.C_Str()))
		{
			pMaterial = ResourceManager::GetInstance()->GetMaterial(matName.C_Str());
		}
		else
		{
			pMaterial = scene->mMaterials[mesh->mMaterialIndex];
			ResourceManager::GetInstance()->AddMaterial(matName.C_Str(), pMaterial);
		}

		ProcessMaterial(pMaterial, scene);
	}
}

void SkinnedMeshRenderer::ProcessBone(aiMesh * mesh, const aiScene * scene)
{
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		UINT boneIndex = 0;
		std::string boneName(mesh->mBones[i]->mName.data);

		//find함수는 해당 원소를 찾지못하면 end()를 반환한다.
		if (boneMapping.find(boneName) == boneMapping.end())
		{
			// 못찾은경우
			boneIndex = boneCount;
			boneCount++;
			BoneInfo bi;
			boneInfo.push_back(bi);
			boneInfo[boneIndex].boneOffset = mesh->mBones[i]->mOffsetMatrix;
			boneMapping[boneName] = boneIndex;
		}
		else
		{
			// 찾은경우
			boneIndex = boneMapping[boneName];
		}
		// mNumWeights : 이 뼈의 영향을 받는 정점들의 수
		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			_bones[mesh->mBones[i]->mWeights[j].mVertexId].AddBoneData(boneIndex, weight); // 현재 boneIndex가 예제와 다름.
			//_bones : 해당하는 정점인덱스 원소로 해당 정점의 뼈 색인과 가중치가 들어감.

			cout << boneIndex << endl; 
		}
	}
}

void SkinnedMeshRenderer::ProcessMaterial(aiMaterial * mat, const aiScene * scene)
{
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);

	for (UINT i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString str;
		string path_texture;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);

		path_texture = string(str.C_Str());
		path_texture = gameObject->directory + '/' + path_texture;

		D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path_texture.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
	}
}

void SkinnedMeshRenderer::SetShader(std::shared_ptr<Shader> _pShader)
{
	pShader = _pShader;
}

void SkinnedMeshRenderer::SetTexturePath(string path)
{
	D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
}

std::shared_ptr<Shader> SkinnedMeshRenderer::GetShader()
{
	return pShader;
}

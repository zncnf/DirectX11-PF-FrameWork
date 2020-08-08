#include "D3DUtil.h"
#include "MeshRenderer.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(GameObject * _object, const aiScene * _pScene, aiNode* _node)
{
	gameObject = _object;
	pScene     = _pScene;
	node       = _node;

	pShader = ResourceManager::GetInstance()->GetShader("DefaultShader_Specular");
}

MeshRenderer::~MeshRenderer()
{
	pShader.reset();
}

void MeshRenderer::Init()
{
	if (pShader)
	{
		ProcessNode(node, pScene);

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

void MeshRenderer::Update()
{
	if (pShader)
	{
		D3DXVECTOR4 diffuse(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0f);
		D3DXVECTOR4 ambient(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
		D3DXVECTOR4 specular(specularColor.r, specularColor.g, specularColor.b, specularColor.a);

		pShader->Update(gameObject->transform->worldMatrix, GameManager::GetInstance()->viewMatrix, GameManager::GetInstance()->projectionMatrix,
			diffuse, ambient, specular, 32.0f);

		DirectXManager::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_ShaderResource);
		DirectXManager::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_SampleState);
	}
}

void MeshRenderer::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
}

void MeshRenderer::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
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

void MeshRenderer::ProcessMaterial(aiMaterial * mat, const aiScene * scene)
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

void MeshRenderer::SetShader(std::shared_ptr<Shader> _pShader)
{
	pShader = _pShader;
}

void MeshRenderer::SetTexturePath(string path)
{
	D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
}

std::shared_ptr<Shader> MeshRenderer::GetShader()
{
	return pShader;
}

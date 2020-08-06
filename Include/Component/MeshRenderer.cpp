#include "D3DUtil.h"
#include "MeshRenderer.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(GameObject * _object, const aiScene * _pScene, aiNode* _node)
{
	gameObject = _object;
	pScene     = _pScene;
	node       = _node;

	pShader = ResourceManager::GetInstance()->getShader("DefaultShader_Specular");

	ProcessNode(node, pScene);
}

MeshRenderer::~MeshRenderer()
{
	pShader.reset();
}

void MeshRenderer::Init()
{
	if (pShader)
	{
		aiString path_astr;
		std::string path_Diffuse;
		std::string path_Specular;
		std::string path_Normals;

		for (UINT i = 0; i < pScene->mNumMaterials; i++)
		{
			if (pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path_astr) == AI_SUCCESS)
			{
				path_Diffuse = path_astr.C_Str();
			}
			else if (pScene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &path_astr) == AI_SUCCESS)
			{
				path_Specular = path_astr.C_Str();
			}
			else if (pScene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path_astr) == AI_SUCCESS)
			{
				path_Normals = path_astr.C_Str();
			}

			if (path_Diffuse.length() != 0)
				break;
			else if (path_Specular.length() != 0)
				break;
			else if (path_Normals.length() != 0)
				break;
		}

		if (path_Diffuse.length() != 0)
		{
			path_Diffuse = "Resource/GameObject/" + path_Diffuse;
			pShader->Init();
			D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path_Diffuse.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
		}
		else if (path_Specular.length() != 0)
		{
			path_Specular = "Resource/GameObject/" + path_Specular;
			pShader->Init();
			D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path_Specular.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
		}
		else if (path_Normals.length() != 0)
		{
			path_Normals = "Resource/GameObject/" + path_Normals;
			pShader->Init();
			D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path_Normals.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
		}
		else
		{
			pShader->Init();
			D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), "Resource/GameObject/box.jpg", nullptr, nullptr, &m_ShaderResource, nullptr);
		}

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
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		ProcessMaterial(mat, scene);
	}
}

void MeshRenderer::ProcessMaterial(aiMaterial * mat, const aiScene * scene)
{
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
}

void MeshRenderer::setShader(std::shared_ptr<Shader> _pShader)
{
	pShader = _pShader;
}

std::shared_ptr<Shader> MeshRenderer::getShader()
{
	return pShader;
}

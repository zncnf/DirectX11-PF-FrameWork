#include "D3DUtil.h"
#include "MeshRenderer.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(GameObject * _object, const aiScene * _pScene)
{
	gameObject = _object;
	pScene = _pScene;

	pShader = ResourceManager::GetInstance()->getShader("DefaultShader_Specular");

	for (UINT i = 0; i < pScene->mNumMaterials; i++)
	{
		pScene->mMaterials[0]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		pScene->mMaterials[0]->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		pScene->mMaterials[0]->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	}
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
		std::string path_str;

		for (UINT i = 0; i < pScene->mNumMaterials; i++)
		{
			pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path_astr);
			path_str = path_astr.C_Str();
			if (path_str.length() != 0)
				break;
		}

		if (path_str.length() != 0)
		{
			path_str = "Resource/GameObject/" + path_str;
			pShader->Init();
			D3DX11CreateShaderResourceViewFromFileA(DirectXManager::GetInstance()->GetDevice(), path_str.c_str(), nullptr, nullptr, &m_ShaderResource, nullptr);
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
		desc.BorderColor[0] = 0; //R
		desc.BorderColor[1] = 0; //G
		desc.BorderColor[2] = 0; //B
		desc.BorderColor[3] = 0; //A
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

void MeshRenderer::setShader(std::shared_ptr<Shader> _pShader)
{
	pShader = _pShader;
}

std::shared_ptr<Shader> MeshRenderer::getShader()
{
	return pShader;
}

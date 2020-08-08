#include "D3DUtil.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::~ResourceManager()
{
	shaders.clear();
	meshs.clear();
	materials.clear();
}

ResourceManager * ResourceManager::GetInstance()
{
	if (!instance)
		instance = new ResourceManager;

	return instance;
}

void ResourceManager::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

shared_ptr<Shader> ResourceManager::GetShader(const std::string filename)
{
	if (shaders.find(filename)->second)
	{
		return shaders.find(filename)->second;
	}

	return nullptr;
}

void ResourceManager::AddShader(string name, shared_ptr<Shader> shader)
{
	shaders.insert(make_pair(name, shader));
}

aiMesh* ResourceManager::GetMesh(const std::string filename)
{
	std::map<string, aiMesh*>::iterator iter;

	for (iter = meshs.begin(); iter != meshs.end(); iter++)
	{
		if (iter->first == filename)
		{
			return iter->second;
		}
	}

	return nullptr;
}

void ResourceManager::AddMesh(string name, aiMesh* mesh)
{
	meshs.insert(make_pair(name, mesh));
}

aiMaterial* ResourceManager::GetMaterial(const std::string filename)
{
	std::map<string, aiMaterial*>::iterator iter;

	for (iter = materials.begin(); iter != materials.end(); iter++)
	{
		if (iter->first == filename)
		{
			return iter->second;
		}
	}

	return nullptr;
}

void ResourceManager::AddMaterial(string name, aiMaterial* material)
{
	materials.insert(make_pair(name, material));
}

void ResourceManager::ShowShaderList()
{
	cout << "\n[Shader List]" << endl;

	std::map<string, shared_ptr<Shader>>::iterator iter;

	for (iter = shaders.begin(); iter != shaders.end(); iter++)
	{
		cout << iter->first << endl;
	}
}

void ResourceManager::ShowMeshList()
{
	cout << "\n[Mesh List]" << endl;

	std::map<string, aiMesh*>::iterator iter;

	for (iter = meshs.begin(); iter != meshs.end(); iter++)
	{
		cout << iter->first << endl;
	}
}

void ResourceManager::ShowMaterialList()
{
	cout << "\n[Material List]" << endl;

	std::map<string, aiMaterial*>::iterator iter;

	for (iter = materials.begin(); iter != materials.end(); iter++)
	{
		cout << iter->first << endl;
	}
}

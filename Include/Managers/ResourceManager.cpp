#include "D3DUtil.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

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

shared_ptr<Shader> ResourceManager::getShader(const std::string filename)
{
	if (shaders.find(filename)->second)
	{
		return shaders.find(filename)->second;
	}

	return nullptr;
}

void ResourceManager::addShader(string name, shared_ptr<Shader> shader)
{
	shaders.insert(make_pair(name, shader));
}

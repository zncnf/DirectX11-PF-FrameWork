#pragma once
#include "Shader/Shader.h"

using namespace std;

class ResourceManager
{
private:
	ResourceManager() = default;
public:
	~ResourceManager() = default;
public:
	static ResourceManager* instance;

	static ResourceManager* GetInstance();
	void   ReleaseInstance();

public:
	shared_ptr<Shader> getShader(const std::string filename);
	void addShader(string name, shared_ptr<Shader> shader);

private:
	map<string, shared_ptr<Shader>> shaders;
};


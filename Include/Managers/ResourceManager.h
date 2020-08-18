#pragma once
#include "Shader/Shader.h"

class Mesh;
class Material;

using namespace std;

class ResourceManager
{
private:
	ResourceManager() = default;
public:
	~ResourceManager();
public:
	static ResourceManager* instance;

	static ResourceManager* GetInstance();
	void   ReleaseInstance();

public:
	shared_ptr<Shader> GetShader(const std::string filename);
	void AddShader(string name, shared_ptr<Shader> shader);

	ID3D11ShaderResourceView* GetShaderResource(const std::string filepath);
	void AddShaderResource(string path, ID3D11ShaderResourceView* shaderResource);

	aiMesh* GetMesh(const std::string filename);
	void AddMesh(string name, aiMesh* mesh);

	aiMaterial* GetMaterial(const std::string filename);
	void AddMaterial(string name, aiMaterial* material);

	void ShowShaderList();
	void ShowMeshList();
	void ShowMaterialList();

private:
	map<string, shared_ptr<Shader>> shaders;
	map<string, aiMesh*> meshs;
	map<string, aiMaterial*> materials;
	map<string, ID3D11ShaderResourceView*> shaderResources;
};


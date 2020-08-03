#pragma once
#include "Structure/GameObject.h"
#include "Shader/DefaultShader_Specular.h"

class Execute
{
public:
	Execute() = default;
	~Execute();

public:
	void Init();
	void Update();

	void SetShaders();
	void Set3DObject();
	void Set2DObject();
	void Render3D();
	void Render2D();

public:
	GameObject* camera;
	GameObject* skull;
};


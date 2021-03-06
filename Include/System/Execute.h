#pragma once
#include "Structure/GameObject.h"

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
	GameObject* directionalLight;
	GameObject* camera;
	GameObject* crusaderKnight;
	GameObject* fps;
	GameObject* mspf;
	GameObject* gameTime;
};


#pragma once
#include "Component.h"
class Camera : public Component
{
public:
	Camera(GameObject* _object);
	virtual ~Camera();

public:
	virtual void Init();
	virtual void Update();
};


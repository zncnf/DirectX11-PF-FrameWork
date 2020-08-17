#pragma once
#include "Structure/Component.h"
class Camera : public Component
{
public:
	Camera(GameObject* _object);
	virtual ~Camera();

public:
	virtual void Init();
	virtual void Update();

public:
	void Pitch(float angle);
	void RotateY(float angle);

private:
	void CameraController();
	void UpdateViewMatrix();
};


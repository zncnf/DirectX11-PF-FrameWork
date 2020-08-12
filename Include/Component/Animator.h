#pragma once
#include "Structure/Component.h"
#include "Asset/AnimationController.h"

class Animator : public Component
{
public:
	Animator(GameObject* _object);
	virtual ~Animator();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void AddController(AnimationController* _controller);

private:
	AnimationController* controller;
};


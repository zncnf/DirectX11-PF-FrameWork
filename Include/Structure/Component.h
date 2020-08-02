#pragma once
#include "Object.h"
#include "GameObject.h"

class Component : public Object
{
public:
	Component();
	virtual ~Component();

public:
	virtual void Init() = 0;
	virtual void Update() = 0;

public:
	GameObject* gameObject;
};


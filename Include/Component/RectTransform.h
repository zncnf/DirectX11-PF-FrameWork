#pragma once
#include "Transform.h"

class RectTransform : public Transform
{
public:
	RectTransform(GameObject* _object);
	virtual ~RectTransform();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	RECT rect;

	void SetRectSize(float width, float height);

public:
	float width;
	float height;
};
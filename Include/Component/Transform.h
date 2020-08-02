#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform() = default;
	Transform(GameObject* _gameObject);
	virtual ~Transform();

public:
	void Init() override;
	void Update() override;

public:
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);

public:
	D3DXVECTOR3 localPosition;
	D3DXVECTOR3 localRotation;
	D3DXVECTOR3 localScale;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 forward;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;

	D3DXMATRIX worldMatrix;
};


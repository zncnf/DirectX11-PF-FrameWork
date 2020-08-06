#pragma once
#include "Structure/Component.h"

enum class LightType
{
	DirectionalLight,
};

class Light : public Component
{
public:
	Light(GameObject* _obejct, LightType _lightType);
	virtual ~Light();

public:
	virtual void Init() override;
	virtual void Update() override;

public:

	void SetLightDirection(D3DXVECTOR3 dir);

private:
	LightType lightType;
	D3DXVECTOR3 lightDirection;

};


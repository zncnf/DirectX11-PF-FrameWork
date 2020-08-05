#include "D3DUtil.h"
#include "Light.h"

Light::Light(GameObject* _obejct, LightType _lightType)
{
	gameObject = _obejct;
	lightType = _lightType;

	switch (_lightType)
	{
	case LightType::DirectionalLight:
	{
		lightDirection = D3DXVECTOR3(0, 0, 1);
		GameManager::GetInstance()->drectionalLight = lightDirection;
	}
		break;
	default:
	{

	}
		break;
	}
}

Light::~Light()
{
}

void Light::Init()
{
}

void Light::Update()
{
}

void Light::SetLightDirection(D3DXVECTOR3 dir)
{
	lightDirection = dir;

	switch (lightType)
	{
	case LightType::DirectionalLight:
	{
		GameManager::GetInstance()->drectionalLight = lightDirection;
	}
		break;
	default:
		break;
	}
}

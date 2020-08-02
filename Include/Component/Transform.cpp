#include "D3DUtil.h"
#include "Transform.h"

Transform::Transform(GameObject * _gameObject)
{
	// pScene에 들어있는 매트릭스 참조해야될듯.

	gameObject = _gameObject;
	gameObject->transform = this;

	position      = D3DXVECTOR3(0, 0, 0);
	rotation      = D3DXVECTOR3(0, 0, 0);
	localScale    = D3DXVECTOR3(1, 1, 1);
	localPosition = D3DXVECTOR3(0, 0, 0);
	localRotation = D3DXVECTOR3(0, 0, 0);

	forward = D3DXVECTOR3(0, 0, 1);
	up	    = D3DXVECTOR3(0, 1, 0);
	right   = D3DXVECTOR3(1, 0, 0);
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update()
{
}

void Transform::Translate(float x, float y, float z)
{
}

void Transform::Rotate(float x, float y, float z)
{
}

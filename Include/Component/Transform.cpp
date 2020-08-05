#include "D3DUtil.h"
#include "Transform.h"

Transform::Transform(GameObject * _gameObject)
{
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

	D3DXMatrixIdentity(&worldMatrix);
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update()
{
	if (gameObject->parent)
	{
		localPosition = position - gameObject->parent->transform->position;
	}
}

void Transform::Translate(D3DXVECTOR3 vec)
{
	position += vec;

	gameObject->transform->worldMatrix._41 = position.x;
	gameObject->transform->worldMatrix._42 = position.y;
	gameObject->transform->worldMatrix._43 = position.z;

	for (int i = 0; i < gameObject->childs.size(); i++)
	{
		gameObject->childs[i]->transform->Translate(vec);
	}
}

void Transform::Rotate(D3DXVECTOR3 vec)
{
	rotation += vec;

	if (rotation.x >= 360)
		rotation.x = 0;
	if (rotation.y >= 360)
		rotation.y = 0;
	if (rotation.z >= 360)
		rotation.z = 0;

	D3DXMATRIX mat, rot;
	D3DXMatrixRotationYawPitchRoll(&rot, static_cast<FLOAT>D3DXToRadian(vec.y), static_cast<FLOAT>D3DXToRadian(vec.x), static_cast<FLOAT>D3DXToRadian(vec.z));
	
	mat = gameObject->transform->worldMatrix * rot;
	
	gameObject->transform->worldMatrix = mat;

	for (int i = 0; i < gameObject->childs.size(); i++)
	{
		gameObject->childs[i]->transform->Rotate(vec);
	}
}


void Transform::Scaling(D3DXVECTOR3 vec)
{
	localScale = vec;

	gameObject->transform->worldMatrix._11 = localScale.x;
	gameObject->transform->worldMatrix._22 = localScale.y;
	gameObject->transform->worldMatrix._33 = localScale.z;

	for (int i = 0; i < gameObject->childs.size(); i++)
	{
		gameObject->childs[i]->transform->Scaling(vec);
	}
}

D3DXVECTOR3 Transform::GetWorldPosition()
{
	return position;
}

void Transform::SetWorldPosition(D3DXVECTOR3 vec)
{
	position = vec;

	gameObject->transform->worldMatrix._41 = position.x;
	gameObject->transform->worldMatrix._42 = position.y;
	gameObject->transform->worldMatrix._43 = position.z;

	for (int i = 0; i < gameObject->childs.size(); i++)
	{
		gameObject->childs[i]->transform->SetWorldPosition(vec);
	}
}

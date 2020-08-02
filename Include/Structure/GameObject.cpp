#include "D3DUtil.h"
#include "GameObject.h"
#include "Component.h"
#include "Component/Transform.h"


GameObject::GameObject(ObjectType objectType)
{
	switch (objectType)
	{
	case ObjectType::Empty:
	{
		active = true;
		gameObject = this;

		this->AddComponent(new Transform(this));
	}
		break;
	case ObjectType::Object3D:
	{

	}
		break;
	case ObjectType::Object2D:
	{

	}
		break;
	case ObjectType::UI:
	{

	}
		break;
	default:
	{

	}
		break;
	}
}

GameObject::~GameObject()
{
	for (int i = 0; i < components.size(); i++)
	{
		Component* compo;
		compo = components[i];

		components[i] = nullptr;

		delete compo;
	}
	components.clear();

	for (int i = 0; i < childs.size(); i++)
	{
		GameObject* child;
		child = childs[i];

		childs[i] = nullptr;

		delete child;
	}
	childs.clear();

	delete parent;
	delete gameObject;
	delete transform;
}

void GameObject::Init()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Init();
	}

	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->Init();
	}
}

void GameObject::Update()
{
	if (parent)
		active = parent->active;

	if (!active)
		return;

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}

	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->Update();
	}
}

void GameObject::AddComponent(Component * _component)
{
	components.push_back(_component);
}

void GameObject::AddChild(GameObject * _gameObject)
{
	childs.push_back(_gameObject);
	_gameObject->parent = this;
}

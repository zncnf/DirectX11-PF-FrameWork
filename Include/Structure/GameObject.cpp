#include "D3DUtil.h"
#include "GameObject.h"
#include "Component.h"
#include "Component/Transform.h"
#include "Component/MeshFilter.h"
#include "Component/MeshRenderer.h"

GameObject::GameObject(ObjectType objectType, const char* filepath)
{
	switch (objectType)
	{
	case ObjectType::Empty:
	{
		active = true;
		pScene = nullptr;
		//gameObject = this; need?

		this->AddComponent(new Transform(this));
	}
		break;
	case ObjectType::Object3D:
	{
		active = true;
		pScene = aiImportFile(filepath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate);
		name = pScene->mRootNode->mName.C_Str();

		this->AddComponent(new Transform(this));
		this->AddComponent(new MeshRenderer(this, pScene));
		this->AddComponent(new MeshFilter(this, pScene, pScene->mRootNode));

		for (UINT i = 0; i < pScene->mRootNode->mNumChildren; i++)
		{
			AddChild(new GameObject(pScene->mRootNode->mChildren[i], pScene));
		}
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

GameObject::GameObject(aiNode * _node, const aiScene * _pScene)
{
	active = true;
	name = _node->mName.C_Str();

	this->AddComponent(new Transform(this));
	this->AddComponent(new MeshRenderer(this, _pScene));
	this->AddComponent(new MeshFilter(this, _pScene, _node));

	for (UINT i = 0; i < _node->mNumChildren; i++)
	{
		AddChild(new GameObject(_node->mChildren[i], _pScene));
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

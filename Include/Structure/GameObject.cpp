#include "D3DUtil.h"
#include "GameObject.h"
#include "Component.h"
#include "Component/Transform.h"
#include "Component/MeshFilter.h"
#include "Component/MeshRenderer.h"
#include "Component/RectTransform.h"
#include "Component/SkinnedMeshRenderer.h"

GameObject::GameObject(ObjectType objectType, string filepath)
{
	switch (objectType)
	{
	case ObjectType::Empty:
	{
		active = true;
		pScene = nullptr;
		
		this->AddComponent(new Transform(this));
	}
		break;
	case ObjectType::Object3D:
	{
		active = true;
		pScene = aiImportFile(filepath.c_str(), ASSIMP_LOAD_FLAG_1);

		name = pScene->mRootNode->mName.C_Str();
		directory = filepath.substr(0, filepath.find_last_of("/"));		
		
		this->AddComponent(new Transform(this));

		if (pScene->mRootNode->mNumMeshes > 0)
		{
			for (UINT i = 0; i < pScene->mRootNode->mNumMeshes; i++)
			{
				if (pScene->mMeshes[pScene->mRootNode->mMeshes[i]]->HasBones())
				{
					this->AddComponent(new SkinnedMeshRenderer(this, pScene, pScene->mRootNode));
					break;
				}
				else
				{
					this->AddComponent(new MeshRenderer(this, pScene, pScene->mRootNode));
					this->AddComponent(new MeshFilter(this, pScene, pScene->mRootNode));
					break;
				}
			}
		}

		for (UINT i = 0; i < pScene->mRootNode->mNumChildren; i++)
		{
			AddChild(new GameObject(pScene->mRootNode->mChildren[i], pScene, filepath));
		}
	}
		break;
	case ObjectType::Object2D:
	{

	}
		break;
	case ObjectType::UI:
	{
		active = true;
		pScene = nullptr;

		this->AddComponent(new RectTransform(this));

		rectTransform = GetComponent<RectTransform>();
	}
		break;
	default:
	{

	}
		break;
	}
}

GameObject::GameObject(aiNode * _node, const aiScene * _pScene, string filepath)
{
	active = true;
	name = _node->mName.C_Str();
	directory = filepath.substr(0, filepath.find_last_of("/"));

	this->AddComponent(new Transform(this));

	if (_node->mNumMeshes > 0)
	{
		for (UINT i = 0; i < _node->mNumMeshes; i++)
		{
			if (_pScene->mMeshes[_node->mMeshes[i]]->HasBones())
			{
				this->AddComponent(new SkinnedMeshRenderer(this, _pScene, _node));
				break;
			}
			else
			{
				this->AddComponent(new MeshRenderer(this, _pScene, _node));
				this->AddComponent(new MeshFilter(this, _pScene, _node));
				break;
			}
		}
	}

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

	aiReleaseImport(pScene);
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

GameObject * GameObject::GetRootObject()
{
	if (this->parent)
	{
		return ParentProcess(this->parent);
	}
	else
	{
		return this;
	}
}

GameObject* GameObject::ParentProcess(GameObject * object)
{
	if (object->parent)
	{
		ParentProcess(object->parent);
	}
	else
	{
		return object;
	}

	return nullptr;
}

void GameObject::AddChild(GameObject * _gameObject)
{
	childs.push_back(_gameObject);
	_gameObject->parent = this;
}

void GameObject::ShowHierarchy(GameObject* _object)
{
	for (int i = 0; i < _object->childs.size(); i++)
	{
		cout << _object->childs[i]->name << endl;
		ShowHierarchy(_object->childs[i]);
	}
}


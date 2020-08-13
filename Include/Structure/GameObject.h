#pragma once
#include "Object.h"

class Component;
class Transform;
class RectTransform;

enum class ObjectType
{
	Empty,
	Object3D,
	Object2D,
	UI,
};

class GameObject : public Object
{
public:
	GameObject(ObjectType _objectType, string filepath = "");
	GameObject(aiNode* _node, const aiScene* _pScene, string filepath = "");
	virtual ~GameObject();

public:
	void Init() override;
	void Update() override;

public:
	void AddComponent(Component* _component);
	template<typename T>
	T* GetComponent();
	template<typename T>
	vector<T*> GetComponentsInChildren();
	void NodeProcess(GameObject* object);

	void AddChild(GameObject* _gameObject);
public:
	GameObject* gameObject       = nullptr;
	GameObject* parent           = nullptr;
	Transform* transform         = nullptr;
	RectTransform* rectTransform = nullptr;
	const aiScene* pScene        = nullptr;

	bool active;
	string directory;

	std::vector<GameObject*> childs;
	std::vector<Component*> components;
};

template<typename T>
inline T * GameObject::GetComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<T*>(components[i]))
		{
			return dynamic_cast<T*>(components[i]);
		}
	}

	return nullptr;
}

template<typename T>
inline vector<T*> GameObject::GetComponentsInChildren()
{
	vector<T*> allChilds;

	for (auto i : this->childs)
	{
		if (i->GetComponent<T>())
		{
			allChilds.push_back(i->GetComponent<T>());
		}
	}

	return allChilds;
}

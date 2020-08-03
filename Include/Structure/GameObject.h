#pragma once
#include "Object.h"

class Component;
class Transform;

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
	GameObject(ObjectType _objectType, const char* filepath = "");
	GameObject(aiNode* _node, const aiScene* _pScene);
	virtual ~GameObject();

public:
	void Init() override;
	void Update() override;

public:
	void AddComponent(Component* _component);
	template<typename T>
	T* GetComponent();

	void AddChild(GameObject* _gameObject);

public:
	GameObject* gameObject = nullptr;
	GameObject* parent     = nullptr;
	Transform* transform   = nullptr;
	const aiScene* pScene  = nullptr;

	bool active;

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

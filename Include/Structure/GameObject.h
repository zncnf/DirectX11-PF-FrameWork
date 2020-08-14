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
	template<typename T>
	vector<T*> GetComponentsInAllChildren();
	template<typename T>
	void NodeProcess(GameObject* object, vector<T*> & vec);

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

//자기 자식의 오브젝트중 형식매개변수로 들어온 컴포넌트타입을 참조해 백터에 담아 반환합니다.
template<typename T>
inline vector<T*> GameObject::GetComponentsInChildren()
{
	vector<T*> childsComponents;

	for (auto i : this->childs)
	{
		if (i->GetComponent<T>())
		{
			childsComponents.push_back(i->GetComponent<T>());
		}
	}

	return childsComponents;
}

//자기의 모든 계층구조를 탐색해 형식매개변수로 들어온 컴포넌트 타입을 참조해 백터에 담아 반환합니다.
template<typename T>
inline vector<T*> GameObject::GetComponentsInAllChildren()
{
	vector<T*> allChildsComponents;

	NodeProcess(this, allChildsComponents);

	return allChildsComponents;
}

//모든 계층구조를 탐색하기위한 재귀함수
template<typename T>
inline void GameObject::NodeProcess(GameObject * object, vector<T*>& vec)
{
	for (UINT i = 0; i < object->childs.size(); i++)
	{
		if (object->childs[i]->GetComponent<T>())
		{
			vec.push_back(object->childs[i]->GetComponent<T>());
		}

		NodeProcess(object->childs[i], vec);
	}
}

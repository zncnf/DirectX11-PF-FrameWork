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
	GameObject* GetRootObject();
	void AddChild(GameObject* _gameObject);
	void ShowHierarchy(GameObject* _object);

private:
	template<typename T>
	void ChildProcess(GameObject* object, vector<T*> & vec);
	GameObject* ParentProcess(GameObject* object);

public:
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

//�ڱ� �ڽ��� ������Ʈ�� ���ĸŰ������� ���� ������ƮŸ���� ������ ���Ϳ� ��� ��ȯ�մϴ�.
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

//�ڱ��� ��� ���������� Ž���� ���ĸŰ������� ���� ������Ʈ Ÿ���� ������ ���Ϳ� ��� ��ȯ�մϴ�.
template<typename T>
inline vector<T*> GameObject::GetComponentsInAllChildren()
{
	vector<T*> allChildsComponents;

	ChildProcess(this, allChildsComponents);

	return allChildsComponents;
}

//��� �ڽ� ���������� Ž���ϱ����� ����Լ�
template<typename T>
inline void GameObject::ChildProcess(GameObject * object, vector<T*>& vec)
{
	for (UINT i = 0; i < object->childs.size(); i++)
	{
		if (object->childs[i]->GetComponent<T>())
		{
			vec.push_back(object->childs[i]->GetComponent<T>());
		}

		ChildProcess(object->childs[i], vec);
	}
}

#include "D3DUtil.h"
#include "GameManager.h"
#include "Structure/GameObject.h"

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance()
{
	if (!instance)
		instance = new GameManager;

	return instance;
}

void GameManager::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void GameManager::Init()
{
	drectionalLight = D3DXVECTOR3(0, 0, 0);
}

void GameManager::ShowHierarchy(GameObject * _object)
{	
	for (int i = 0; i < _object->childs.size(); i++)
	{
		cout << _object->childs[i]->name << endl;
		ShowHierarchy(_object->childs[i]);
	}
}

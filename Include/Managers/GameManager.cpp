#include "D3DUtil.h"
#include "GameManager.h"

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
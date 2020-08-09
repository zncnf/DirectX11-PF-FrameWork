#include "D3DUtil.h"
#include "GameManager.h"
#include "Structure/GameObject.h"
#include "System/GameTimer.h"

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
		delete gameTimer;
		delete instance;
		gameTimer = nullptr;
		instance = nullptr;
	}
}

void GameManager::Init()
{
	gameTimer = new GameTimer();
	gameTimer->Reset();

	drectionalLight = D3DXVECTOR3(0, 0, 0);
}

void GameManager::Update()
{
	gameTimer->Tick();

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((gameTimer->GameTime() - timeElapsed) >= 1.0f)
	{
		this->fps = (float)frameCnt;
		this->mspf = 1000.0f / this->fps; // 한프레임 을 처리하는데 걸리는 시간(밀리초 단위)
		this->gameTime = gameTimer->GameTime();

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void GameManager::ShowHierarchy(GameObject * _object)
{	
	for (int i = 0; i < _object->childs.size(); i++)
	{
		cout << _object->childs[i]->name << endl;
		ShowHierarchy(_object->childs[i]);
	}
}

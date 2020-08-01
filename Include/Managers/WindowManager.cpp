#include "D3DUtil.h"
#include "WindowManager.h"

WindowManager* WindowManager::instance = nullptr;

WindowManager * WindowManager::GetInstance()
{
	if (!instance)
		instance = new WindowManager;

	return instance;
}

void WindowManager::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

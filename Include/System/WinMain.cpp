#include "D3DUtil.h"
#include "WinMain.h"

#include "Execute.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE previnstacne, LPSTR ipszCmdParam, int nCmdShow)
{
	//Setup Program
	g_instance = hInstance;
	SetUpWindow(1280, 720);

	#pragma region SetManagers
	RECT rect;
	GetClientRect(g_handle, &rect);
	WindowManager::GetInstance()->SetHInstance(g_instance);
	WindowManager::GetInstance()->SetWindowHandle(g_handle);
	WindowManager::GetInstance()->SetWindowWidth(static_cast<float>(rect.right - rect.left));
	WindowManager::GetInstance()->SetWindowHeight(static_cast<float>(rect.bottom - rect.top));

	DirectXManager::GetInstance()->Init();
	#pragma endregion

	Execute* execute = new Execute();
	execute->Init();

	#pragma region Update
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DirectXManager::GetInstance()->BeginDraw();
			{
				execute->Update();
			}
			DirectXManager::GetInstance()->EndDraww();
		}
	}
	#pragma endregion

	#pragma region Released Before DestroyWindow
	WindowManager::GetInstance()->ReleaseInstance();
	DirectXManager::GetInstance()->ReleaseInstance();
	GameManager::GetInstance()->ReleaseInstance();
	delete execute;
	#pragma endregion
	
	//Escape Program
	DestroyWindow(g_handle);
	UnregisterClass(L"D3D11 Engine", g_instance);

	return 0;
}
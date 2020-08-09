#pragma once
#include "GameTimer.h"

static HINSTANCE g_instance;
static HWND      g_handle;

inline LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			if (!GameManager::GetInstance()->gameTimer)
				GameManager::GetInstance()->Init();
			GameManager::GetInstance()->gameTimer->Stop();
		}
		else
		{
			if (!GameManager::GetInstance()->gameTimer)
				GameManager::GetInstance()->Init();
			GameManager::GetInstance()->gameTimer->Start();
		}
		return 0;
	}
		break;
	case WM_ENTERSIZEMOVE:
	{
		GameManager::GetInstance()->gameTimer->Stop();
		return 0;
	}
		break;
	case WM_EXITSIZEMOVE:
	{
		GameManager::GetInstance()->gameTimer->Start();
		return 0;
	}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline void SetUpWindow(const UINT& width, const UINT& height)
{
	WNDCLASSEX wnd_class;

	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = 0;
	wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wnd_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wnd_class.hIcon         = LoadIcon(nullptr, IDI_ERROR);
	wnd_class.hIconSm       = LoadIcon(nullptr, IDI_ERROR);
	wnd_class.hInstance     = g_instance;
	wnd_class.lpfnWndProc   = WndProc;
	wnd_class.lpszClassName = L"D3D11 Engine";
	wnd_class.lpszMenuName  = nullptr;
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
	wnd_class.cbSize        = sizeof(WNDCLASSEX);

	RegisterClassEx(&wnd_class);

	g_handle = CreateWindowExW(WS_EX_APPWINDOW, L"D3D11 Engine", L"D3D11 Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		static_cast<int>(width), static_cast<int>(height), nullptr, nullptr, g_instance, nullptr);

	assert(g_handle != nullptr);

	SetForegroundWindow(g_handle);
	SetFocus(g_handle);
	ShowCursor(TRUE);
	ShowWindow(g_handle, SW_NORMAL);
	UpdateWindow(g_handle);
}
#pragma once

class WindowManager final
{
private:
	WindowManager()  = default;
public:
	~WindowManager() = default;

	HINSTANCE       GetHInstance()					      { return hInstance; }
	void            SetHInstance(HINSTANCE _hinstance)    { hInstance = _hinstance; }
				      								    
	HWND	        GetWindowHandle()				      { return hWinHandle; }
	void	        SetWindowHandle(HWND _hWinHandle)     { hWinHandle = _hWinHandle; }
												    
	const float&    GetWindowWidth()				      { return width; }
	void		    SetWindowWidth(const float& _width)   { width = _width; }

	const float&    GetWindowHeight()				      { return height; }
	void            SetWindowHeight(const float& _height) { height = _height; }

public:
	static WindowManager* instance;

	static WindowManager* GetInstance();
	void   ReleaseInstance();

private:
	HINSTANCE hInstance;
	HWND	  hWinHandle;

	float width;
	float height;
};


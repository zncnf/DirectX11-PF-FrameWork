#pragma once
class InputManager
{
private:
	InputManager() = default;
public:
	~InputManager() = default;
public:
	static InputManager* instance;

	static InputManager* GetInstance();
	void   ReleaseInstance();

public:
	HRESULT Init();
	void Update();
	void ShowDown();

	bool IsEscapePressed();
	POINT GetMouseLocation();
	bool KeyboardDown(int inputType);
	bool KeyboardUp(int inputType);
	bool MouseDown(int inputType);
	bool MouseDownOnce(int inputType);
	bool MouseUp(int inputType);
	bool MouseUpOnce(int inputType);
public:
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_prevMouseState;
	DIMOUSESTATE m_prevMouseState2;
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];


	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};


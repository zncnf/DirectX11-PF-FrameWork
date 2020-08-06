#include "D3DUtil.h"
#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (!instance)
		instance = new InputManager;

	return instance;
}

void InputManager::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

HRESULT InputManager::Init()
{
	HRESULT result;

	m_mouseX = 0;
	m_mouseY = 0;

	m_screenWidth = static_cast<int>(WindowManager::GetInstance()->GetWindowWidth());
	m_screenHeight = static_cast<int>(WindowManager::GetInstance()->GetWindowHeight());

	result = DirectInput8Create(WindowManager::GetInstance()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
		return false;

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return false;

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	result = m_keyboard->SetCooperativeLevel(WindowManager::GetInstance()->GetWindowHandle(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	result = m_keyboard->Acquire();
	if (FAILED(result))
		return false;

	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
		return false;

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	result = m_mouse->SetCooperativeLevel(WindowManager::GetInstance()->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	result = m_mouse->Acquire();
	if (FAILED(result))
		return false;

	return S_OK;
}

void InputManager::Update()
{
	ReadKeyboard();
	ReadMouse();
	ProcessInput();
}

void InputManager::ShowDown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputManager::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

POINT InputManager::GetMouseLocation()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(WindowManager::GetInstance()->GetWindowHandle(), &pt);

	if (pt.x < 0) { pt.x = 0; }
	if (pt.y < 0) { pt.y = 0; }

	if (pt.x > m_screenWidth) { pt.x = m_screenWidth; }
	if (pt.y > m_screenHeight) { pt.y = m_screenHeight; }

	return pt;
}

bool InputManager::KeyboardDown(int inputType)
{
	if (m_keyboardState[inputType] & 0x80)
		return true;

	return false;
}

bool InputManager::KeyboardUp(int inputType)
{
	if (m_keyboardState[inputType] & 0x80)
		return false;

	return true;
}

bool InputManager::MouseDown(int inputType)
{
	if (m_mouseState.rgbButtons[inputType] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::MouseDownOnce(int inputType)
{
	if (m_mouseState.rgbButtons[inputType] & 0x80)
	{
		if (m_prevMouseState.rgbButtons[inputType])
			return false;
		else
		{
			m_prevMouseState.rgbButtons[inputType] = 1;
			return true;
		}
	}
	else
	{
		m_prevMouseState.rgbButtons[inputType] = 0;
		return false;
	}
}

bool InputManager::MouseUp(int inputType)
{
	if (m_mouseState.rgbButtons[inputType] & 0x80)
		return false;

	return true;
}

bool InputManager::MouseUpOnce(int inputType)
{
	if (m_mouseState.rgbButtons[inputType] & 0x80)
	{
		m_prevMouseState2.rgbButtons[inputType] = 1;
		return false;
	}

	if (!(m_mouseState.rgbButtons[inputType] & 0x80) && m_prevMouseState2.rgbButtons[inputType] == 1)
	{
		m_prevMouseState2.rgbButtons[inputType] = 0;
		return true;
	}

	return false;
}

bool InputManager::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState); if (FAILED(result))

		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_keyboard->Acquire();
		else
			return false;

	return true;
}

bool InputManager::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState); if (FAILED(result))
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse->Acquire();
		else
			return false;

	return true;
}

void InputManager::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}


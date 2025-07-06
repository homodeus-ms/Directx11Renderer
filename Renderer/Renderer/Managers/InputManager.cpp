#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_eachKeyStates.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::Update()
{
	if (IsWindowFocused())
	{
		UpdateKeys();
		UpdateMouse();
	}
	else
	{
		LOG(Log, "Window Not Focused");
		ClearKeyStateAll();
	}
}

bool InputManager::IsWindowFocused()
{
	HWND hwnd = ::GetActiveWindow();

	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_eachKeyStates[key] = KeyState::None;

		return false;
	}

	return true;
}

void InputManager::UpdateKeys()
{
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (asciiKeys[key] & 0x80)  // Key Pressed true
		{
			KeyState& state = _eachKeyStates[key];
			
			// Key State pre-Frame
			if (state == KeyState::Pressed || state == KeyState::Hold)
				state = KeyState::Hold;
			else
				state = KeyState::Pressed;
		}
		else						// Key Pressed false
		{
			KeyState& state = _eachKeyStates[key];

			// Key State pre-Frame
			if (state == KeyState::Pressed || state == KeyState::Hold)
				state = KeyState::Released;
			else
				state = KeyState::None;
		}
	}
}

void InputManager::UpdateMouse()
{
	::GetCursorPos(&_mousePos);
	::ScreenToClient(_hwnd, &_mousePos);
}

void InputManager::ClearKeyStateAll()
{
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
		_eachKeyStates[key] = KeyState::None;
}

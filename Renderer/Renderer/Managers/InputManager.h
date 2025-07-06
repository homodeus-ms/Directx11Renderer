#pragma once

enum class KeyType
{
	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	Q = 'Q',
	E = 'E',
	Z = 'Z',
	C = 'C',

	Key1 = '1',
	Key2 = '2',
	Key3 = '3',
	Key4 = '4',

	MouseL = VK_LBUTTON,
	MouseR = VK_RBUTTON,
};

enum class KeyState
{
	None,
	Pressed,
	Hold,
	Released,
	End
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int32>(KeyState::End),
};

class InputManager
{
	DECLARE_SINGLE(InputManager);

public:
	void Init(HWND hwnd);
	void Update();
	
	bool ButtonPressed(KeyType key) { return GetState(key) == KeyState::Pressed; }
	bool ButtonHold(KeyType key) { return GetState(key) == KeyState::Hold; }
	bool ButtonReleased(KeyType key) { return GetState(key) == KeyState::Released; }

	const POINT& GetMousePos() { return _mousePos; }

	bool IsWindowFocused();

private:
	inline KeyState GetState(KeyType key) { return _eachKeyStates[static_cast<uint8>(key)]; }
	void UpdateKeys();
	void UpdateMouse();
	void ClearKeyStateAll();

	HWND _hwnd{};
	vector<KeyState> _eachKeyStates;
	POINT _mousePos = {};
};


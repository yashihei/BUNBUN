#include "Input.h"

#include <stdexcept>

Keyboard::Keyboard(LPDIRECTINPUT8 directInput, HWND hWnd) :
m_directInputDevice(NULL)
{
	HRESULT hr = directInput->CreateDevice(GUID_SysKeyboard, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device(Keyboard)");

	//データ形式の設定
	hr = m_directInputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		throw std::runtime_error("Error setDataFormat");
	//協調レベルの設定
	hr = m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		throw std::runtime_error("Error setCooperativeLevel");

	//キーボード入力を有効化
	m_directInputDevice->Acquire();
}

Keyboard::~Keyboard() {
	if (m_directInputDevice) {
		m_directInputDevice->Unacquire();
		m_directInputDevice->Release();
	}
}

void Keyboard::updateState() {
	std::array<BYTE, KeyNum> buf;
	HRESULT hr = m_directInputDevice->GetDeviceState(buf.size(), buf.data());

	if (FAILED(hr)) {
		m_directInputDevice->Acquire();
		return;
	}

	for (int i = 0; i < KeyNum; i++) {
		m_state[i][Press] = (buf[i] & 0x80) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
	}
}

//MEMO:DirectInputだと、タブレットではマウスとして扱ってくれない
Mouse::Mouse(LPDIRECTINPUT8 directInput, HWND hWnd) :
m_directInputDevice(NULL), m_hWnd(hWnd), m_cursorPos(0, 0)
{
	HRESULT hr = directInput->CreateDevice(GUID_SysMouse, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device(Mouse)");

	//mouse→ボタン4つ mouse2→最大ボタン8つ
	hr = m_directInputDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		throw std::runtime_error("Error setDataFormat");

	hr = m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		throw std::runtime_error("Error setCooperativeLevel");

	m_directInputDevice->Acquire();
}

Mouse::~Mouse() {
	if (m_directInputDevice) {
		m_directInputDevice->Unacquire();
		m_directInputDevice->Release();
	}
}

void Mouse::updateState() {
	DIMOUSESTATE mouseState = {};
	HRESULT hr = m_directInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

	if (FAILED(hr)) {
		m_directInputDevice->Acquire();
		return;
	}

	for (int i = 0; i < buttonNum; i++) {
		m_state[i][Press] = (mouseState.rgbButtons[i] & 0x80) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
	}

	//DirectInputでは相対値の座標しかとれないので、win32apiを使う
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_hWnd, &cursorPos);
	m_cursorPos.x = cursorPos.x;
	m_cursorPos.y = cursorPos.y;
}

//MEMO:XInput、win7ではlibのバージョンで動かないことがあるらしいので注意
XInput::XInput(int index) :
m_index(index)
{
	m_state.fill(std::bitset<StateNum>());
}

void XInput::updateState() {
	auto res = XInputGetState(m_index, &m_xInputState);
	if (res == ERROR_DEVICE_NOT_CONNECTED) {
		m_state.fill(std::bitset<StateNum>());
		return;
	}

	int buttonIndex = 1;
	for (int i = 0; i < buttonNum; i++) {
		m_state[i][Press] = (m_xInputState.Gamepad.wButtons & buttonIndex) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
		buttonIndex *= (buttonIndex != 0x0200) ? 2 : 8;
	}

	//check deadzone
	const SHORT leftDeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	if ((m_xInputState.Gamepad.sThumbLX < leftDeadZone && m_xInputState.Gamepad.sThumbLX > -leftDeadZone) &&
		(m_xInputState.Gamepad.sThumbLY < leftDeadZone && m_xInputState.Gamepad.sThumbLY > -leftDeadZone))
	{
		m_xInputState.Gamepad.sThumbLX = 0;
		m_xInputState.Gamepad.sThumbLY = 0;
	}
	const SHORT rightDeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	if ((m_xInputState.Gamepad.sThumbRX < rightDeadZone && m_xInputState.Gamepad.sThumbRX > -rightDeadZone) &&
		(m_xInputState.Gamepad.sThumbRY < rightDeadZone && m_xInputState.Gamepad.sThumbRY > -rightDeadZone))
	{
		m_xInputState.Gamepad.sThumbRX = 0;
		m_xInputState.Gamepad.sThumbRY = 0;
	}
}

//-1.0~1.0までの正規化した値を返す
float normalized(SHORT n) {
	return static_cast<float>(n) / 0x7FFF;
}

Vector2 XInput::getLeftThumb() {
	return{ normalized(m_xInputState.Gamepad.sThumbLX), -normalized(m_xInputState.Gamepad.sThumbLY) };
}

Vector2 XInput::getRightThumb() {
	return{ normalized(m_xInputState.Gamepad.sThumbRX), -normalized(m_xInputState.Gamepad.sThumbRY) };
}

bool XInput::isConnecting() {
	auto res = XInputGetState(m_index, &m_xInputState);
	if (res == ERROR_DEVICE_NOT_CONNECTED)
		return false;
	return true;
}

InputManager::InputManager(HWND hWnd, HINSTANCE hInstance) {
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize DirectInput");

	m_keyboard = std::make_shared<Keyboard>(m_directInput, hWnd);
	m_mouse = std::make_shared<Mouse>(m_directInput, hWnd);
	m_xInput = std::make_shared<XInput>();
}

InputManager::~InputManager() {
	if (m_directInput)
		m_directInput->Release();
}

void InputManager::update() {
	m_keyboard->updateState();
	m_mouse->updateState();
	m_xInput->updateState();
}

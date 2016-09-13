#pragma once
#include <Windows.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <bitset>
#include <array>
#include <memory>
#include <XInput.h>
#include "Point.h"
#include "Vector2.h"

//TODO:–¼‘O‹óŠÔ‚ÅˆÍ‚Þ
enum State {
	Prev,
	Press,
	Click,
	Release,
	StateNum
};

class Keyboard {
public:
	Keyboard(LPDIRECTINPUT8 directInput, HWND hWnd);
	~Keyboard();
	void updateState();
	bool isClicked(BYTE code) { return m_state[code][Click]; }
	bool isPressed(BYTE code) { return m_state[code][Press]; }
	bool isReleased(BYTE code) { return m_state[code][Release]; }
private:
	static const int KeyNum = 256;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
	std::array<std::bitset<StateNum>, KeyNum> m_state;
};

class Mouse {
public:
	enum Button {
		Left,
		Right,
		Center,
	};
	Mouse(LPDIRECTINPUT8 directInput, HWND hWnd);
	~Mouse();
	void updateState();
	bool isClicked(Button button) { return m_state[button][Click]; }
	bool isPressed(Button button) { return m_state[button][Press]; }
	bool isReleased(Button button) { return m_state[button][Release]; }
	Point getCursorPos() { return m_cursorPos; }
private:
	static const int buttonNum = 3;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
	HWND m_hWnd;
	Point m_cursorPos;
	std::array<std::bitset<StateNum>, buttonNum> m_state;
};

class XInput {
public:
	enum Button {
		Up,
		Down,
		Left,
		Right,
		Start,
		Back,
		LS,
		RS,
		LB,
		RB,
		A, B, X, Y,
	};

	XInput(int index = 0);
	void updateState();
	bool isClicked(Button button) { return m_state[button][Click]; }
	bool isPressed(Button button) { return m_state[button][Press]; }
	bool isReleased(Button button) { return m_state[button][Release]; }
	Vector2 getLeftThumb();
	Vector2 getRightThumb();
	bool isConnecting();
private:
	static const int buttonNum = 14;
	int m_index;
	std::array<std::bitset<StateNum>, buttonNum> m_state;
	XINPUT_STATE m_xInputState;
};

class InputManager {
public:
	InputManager(HWND hWnd, HINSTANCE hInstance);
	~InputManager();
	void update();

	bool isPressedButton1() const { return m_keyboard->isPressed(DIK_Z) | m_xInput->isPressed(XInput::Y); }
	bool isPressedButton2() const { return m_keyboard->isPressed(DIK_X) | m_xInput->isPressed(XInput::B); }
	bool isPressedButton3() const { return m_keyboard->isPressed(DIK_C) | m_xInput->isPressed(XInput::A); }
	bool isPressedButton4() const { return m_keyboard->isPressed(DIK_V) | m_xInput->isPressed(XInput::X); }
	bool isPressedUp()      const { return m_keyboard->isPressed(DIK_UP) | m_xInput->isPressed(XInput::Up); }
	bool isPressedDown()    const { return m_keyboard->isPressed(DIK_DOWN) | m_xInput->isPressed(XInput::Down); }
	bool isPressedLeft()    const { return m_keyboard->isPressed(DIK_LEFT) | m_xInput->isPressed(XInput::Left); }
	bool isPressedRight()   const { return m_keyboard->isPressed(DIK_RIGHT) | m_xInput->isPressed(XInput::Right); }

	bool isClickedButton() const { return isClickedButton1() | isClickedButton2() | isClickedButton3() | isClickedButton4(); }
	bool isClickedButton1() const { return m_keyboard->isClicked(DIK_Z) | m_xInput->isClicked(XInput::Y); }
	bool isClickedButton2() const { return m_keyboard->isClicked(DIK_X) | m_xInput->isClicked(XInput::B); }
	bool isClickedButton3() const { return m_keyboard->isClicked(DIK_C) | m_xInput->isClicked(XInput::A); }
	bool isClickedButton4() const { return m_keyboard->isClicked(DIK_V) | m_xInput->isClicked(XInput::X); }
	bool isClickedUp()      const { return m_keyboard->isClicked(DIK_UP) | m_xInput->isClicked(XInput::Up); }
	bool isClickedDown()    const { return m_keyboard->isClicked(DIK_DOWN) | m_xInput->isClicked(XInput::Down); }
	bool isClickedLeft()    const { return m_keyboard->isClicked(DIK_LEFT) | m_xInput->isClicked(XInput::Left); }
	bool isClickedRight()   const { return m_keyboard->isClicked(DIK_RIGHT) | m_xInput->isClicked(XInput::Right); }

	bool isReleasedButton1() const { return m_keyboard->isReleased(DIK_Z) | m_xInput->isReleased(XInput::Y); }
	bool isReleasedButton2() const { return m_keyboard->isReleased(DIK_X) | m_xInput->isReleased(XInput::B); }
	bool isReleasedButton3() const { return m_keyboard->isReleased(DIK_C) | m_xInput->isReleased(XInput::A); }
	bool isReleasedButton4() const { return m_keyboard->isReleased(DIK_V) | m_xInput->isReleased(XInput::X); }
	bool isReleasedUp()      const { return m_keyboard->isReleased(DIK_UP) | m_xInput->isReleased(XInput::Up); }
	bool isReleasedDown()    const { return m_keyboard->isReleased(DIK_DOWN) | m_xInput->isReleased(XInput::Down); }
	bool isReleasedLeft()    const { return m_keyboard->isReleased(DIK_LEFT) | m_xInput->isReleased(XInput::Left); }
	bool isReleasedRight()   const { return m_keyboard->isReleased(DIK_RIGHT) | m_xInput->isReleased(XInput::Right); }

	Vector2 getAxis();

	std::shared_ptr<Keyboard> getKeyboard() { return m_keyboard; }
	std::shared_ptr<Mouse> getMouse() { return m_mouse; }
	std::shared_ptr<XInput> getXInput() { return m_xInput; }
private:
	LPDIRECTINPUT8 m_directInput;
	std::shared_ptr<Keyboard> m_keyboard;
	std::shared_ptr<Mouse> m_mouse;
	std::shared_ptr<XInput> m_xInput;
};

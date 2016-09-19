#pragma once
#include <Windows.h>
#include <memory>
#include "Fwd.h"

class GameApp {
public:
	GameApp(HWND hWnd, HINSTANCE hInstance, bool fullScreen);
	virtual ~GameApp() = default;
	void run();
protected:
	virtual void update() = 0;
	virtual void draw() = 0;

	GraphicDevicePtr m_graphicDevice;
	TextureMgrPtr m_textureManager;
	SoundMgrPtr m_soundManager;
	InputMgrPtr m_inputManager;
	FPSControlerPtr m_fpsControler;
	RandomPtr m_random;
	int m_frameCount;
};

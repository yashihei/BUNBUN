#pragma once
#include <Windows.h>
#include <memory>

class GraphicDevice;
class TextureManager;
class SoundManager;
class InputManager;
class FPSControler;
class Random;

using GraphicDevicePtr = std::shared_ptr<GraphicDevice>;
using TextureMgrPtr = std::shared_ptr<TextureManager>;
using SoundMgrPtr = std::shared_ptr<SoundManager>;
using InputMgrPtr = std::shared_ptr<InputManager>;
using FPSControlerPtr = std::shared_ptr<FPSControler>;
using RandomPtr = std::shared_ptr<Random>;

class GameApp {
public:
	GameApp(HWND hWnd, HINSTANCE hInstance);
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

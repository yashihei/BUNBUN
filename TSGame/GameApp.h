#pragma once
#include <Windows.h>
#include <memory>

class GraphicDevice;
class TextureManager;
class SoundManager;
class InputManager;
class FPSControler;
class Random;

class GameApp {
public:
	GameApp(HWND hWnd, HINSTANCE hInstance);
	virtual ~GameApp() = default;
	void run();
protected:
	virtual void update() = 0;
	virtual void draw() = 0;

	std::shared_ptr<GraphicDevice> m_graphicDevice;
	std::shared_ptr<TextureManager> m_textureManager;
	std::shared_ptr<SoundManager> m_soundManager;
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<FPSControler> m_fpsControler;
	std::shared_ptr<Random> m_random;
	int m_frameCount;
};

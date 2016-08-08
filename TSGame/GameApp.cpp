#include "GameApp.h"

#include "Graphic.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"

GameApp::GameApp(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0)
{
	m_graphicDevice = std::make_shared<GraphicDevice>(hWnd);
	m_textureManager = std::make_shared<TextureManager>(m_graphicDevice->getDevice());
	m_soundManager = std::make_shared<SoundManager>();
	m_inputManager = std::make_shared<InputManager>(hWnd, hInstance);
	m_fpsControler = std::make_shared<FPSControler>(60);
}

void GameApp::run() {
	m_frameCount++;
	m_inputManager->update();
	update();
	if (SUCCEEDED(m_graphicDevice->beginScene())) {
		draw();
		m_graphicDevice->endScene();
	}
	m_graphicDevice->present();
	m_fpsControler->wait();
}

#include "TSGame.h"

#include "Font.h"
#include "Sound.h"
#include "Title.h"
#include "Play.h"

TSGame::TSGame(HWND hWnd, HINSTANCE hInstance, bool fullScreen) :
GameApp(hWnd, hInstance, fullScreen)
{
	Font::addFont("dat/orbitron-medium.otf");
	m_soundManager->load("dat/kin.wav", "kin");
	m_soundManager->load("dat/zan.wav", "zan");
	m_soundManager->load("dat/giun.wav", "giun");
	m_soundManager->load("dat/bgm.wav", "bgm");
	changeScene(SceneType::Title);
}

void TSGame::update() {
	m_scene->update();
	if (m_scene->nextScene() != SceneType::None)
		changeScene(m_scene->nextScene());
}

void TSGame::draw() {
	m_scene->draw();
}

void TSGame::changeScene(SceneType type) {
	switch (type) {
	case SceneType::Title:
		m_scene = std::make_shared<Title>(m_graphicDevice, m_soundManager, m_inputManager);
		break;
	case SceneType::Play:
		m_scene = std::make_shared<Play>(m_graphicDevice, m_soundManager, m_inputManager, m_random);
		break;
	case SceneType::Result:
		break;
	}
}

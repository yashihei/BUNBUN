#include "TSGame.h"

#include "Font.h"
#include "Sound.h"
#include "Title.h"
#include "Play.h"

TSGame::TSGame(HWND hWnd, HINSTANCE hInstance) :
GameApp(hWnd, hInstance)
{
	Font::addFont("dat/orbitron-medium.otf");
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

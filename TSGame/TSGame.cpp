#include "TSGame.h"

#include "Graphic.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"
#include "Util.h"
#include "Font.h"
#include "Actors.h"
#include "Shape.h"

TSGame::TSGame(HWND hWnd, HINSTANCE hInstance) :
GameApp(hWnd, hInstance) 
{
	m_player = std::make_shared<Player>(this, m_inputManager, m_graphicDevice->getDevice());
	m_enemies = std::make_shared<ActorManager<Enemy>>();
}

void TSGame::update() {
	if (m_random->next(60) == 0) {
		auto enemy = std::make_shared<Enemy>(Vector2(m_random->next(640.0f), m_random->next(480.0f)), this, m_graphicDevice->getDevice());
		m_enemies->add(enemy);
	}
	m_player->update();
	m_enemies->update();
}

void TSGame::draw() {
	for (int i = 0; i < 16; i++)
		Shape::drawLine(m_graphicDevice->getDevice(), { i*40.0f, 0.0f }, { i*40.0f, 480.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	for (int i = 0; i < 12; i++)
		Shape::drawLine(m_graphicDevice->getDevice(), { 0.0f, i*40.0f }, { 640.f, i*40.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	m_player->draw();
	m_enemies->draw();
}

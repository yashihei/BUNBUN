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
	m_player = std::make_shared<Player>(m_inputManager, m_graphicDevice->getDevice());
	m_flail = std::make_shared<Flail>(m_player, m_graphicDevice->getDevice());
	m_enemies = std::make_shared<ActorManager<Enemy>>();
}

bool isHit(Vector2 pos1, Vector2 pos2, float radius1, float radius2) {
	if (std::pow(pos1.x - pos2.x, 2.0f) + std::pow(pos1.y - pos2.y, 2.0f) <= std::pow(radius1 + radius2, 2.0f))
		return true;
	return false;
}

void TSGame::update() {
	if (m_random->next(60) == 0) {
		auto enemy = std::make_shared<Enemy>(Vector2(m_random->next(640.0f), m_random->next(480.0f)), m_player, m_graphicDevice->getDevice());
		m_enemies->add(enemy);
	}
	m_player->update();
	m_flail->update();
	m_enemies->update();

	for (auto& enemy : *m_enemies) {
		if (isHit(enemy->getPos(), m_flail->getPos(), 20.f, m_flail->getRadius() - 5))
			enemy->kill();
	}
}

void TSGame::draw() {
	for (int i = 0; i < 16; i++)
		Shape::drawLine(m_graphicDevice->getDevice(), { i*40.0f, 0.0f }, { i*40.0f, 480.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	for (int i = 0; i < 12; i++)
		Shape::drawLine(m_graphicDevice->getDevice(), { 0.0f, i*40.0f }, { 640.f, i*40.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	m_player->draw();
	m_flail->draw();
	m_enemies->draw();
}

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
#include "Color.h"

TSGame::TSGame(HWND hWnd, HINSTANCE hInstance) :
GameApp(hWnd, hInstance), m_score(0)
{
	m_player = std::make_shared<Player>(m_inputManager, m_graphicDevice->getDevice());
	m_flail = std::make_shared<Flail>(m_player, m_graphicDevice->getDevice());
	m_enemies = std::make_shared<ActorManager<Enemy>>();
	m_effects = std::make_shared<ActorManager<Effect>>();
	
	Font::addFont("dat/orbitron-medium.otf");
	m_hudFont = std::make_shared<Font>(20, "Orbitron", false, m_graphicDevice->getDevice());
}

bool isHit(Vector2 pos1, Vector2 pos2, float radius1, float radius2) {
	if (std::pow(pos1.x - pos2.x, 2.0f) + std::pow(pos1.y - pos2.y, 2.0f) <= std::pow(radius1 + radius2, 2.0f))
		return true;
	return false;
}

void TSGame::update() {
	if (m_random->next(60) == 0) {
		//プレイヤーから離れた場所に出現させる
		Vector2 pos;
		while (true) {
			pos = Vector2(m_random->next(640.0f), m_random->next(480.0f));
			auto dis = pos - m_player->getPos();
			if (std::abs(dis.length()) > 100)
				break;
		}
		auto enemy = std::make_shared<Enemy>(pos, m_player, m_graphicDevice->getDevice());
		m_enemies->add(enemy);
	}
	m_player->update();
	m_flail->update();
	m_enemies->update();
	m_effects->update();

	for (auto& enemy : *m_enemies) {
		//if (isHit(enemy->getPos(), m_player->getPos(), 15.0f, 10.0f))
		if (!enemy->start() && isHit(enemy->getPos(), m_flail->getPos(), 15.0f, m_flail->getRadius())) {
			enemy->kill();
			m_score += 100;
			auto effect = std::make_shared<Effect>(enemy->getPos(), m_graphicDevice->getDevice());
			m_effects->add(effect);
		}
	}
}

void TSGame::draw() {
	for (int i = 0; i < 16; i++)
		Shape::drawLine(m_graphicDevice->getDevice(), { i*40.0f, 0.0f }, { i*40.0f, 480.0f }, 1.0f, Color(1.0f, 1.0f, 1.0f, 0.125f).toD3Dcolor());
	for (int i = 0; i < 12; i++)
		Shape::drawLine(m_graphicDevice->getDevice(), { 0.0f, i*40.0f }, { 640.f, i*40.0f }, 1.0f, Color(1.0f, 1.0f, 1.0f, 0.125f).toD3Dcolor());
	m_player->draw();
	m_flail->draw();
	m_enemies->draw();
	m_effects->draw();
	m_hudFont->drawStr("SCORE " + std::to_string(m_score), { 10, 10 }, Color(0.6f, 1.0f, 0.6f, 1.0f).toD3Dcolor());
}

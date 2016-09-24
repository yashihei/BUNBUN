#include "Play.h"

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
#include "Easing.h"

Play::Play(GraphicDevicePtr graphic, SoundMgrPtr soundManager, InputMgrPtr inputManager, RandomPtr random) :
m_graphicDevice(graphic), m_soundManager(soundManager), m_inputManager(inputManager), m_random(random),
m_frameCount(0), m_gameoverCount(0), m_score(0), m_viewScore(0), m_level(1)
{
	m_player = std::make_shared<Player>(m_inputManager, m_graphicDevice->getDevice());
	m_flail = std::make_shared<Flail>(m_player, m_graphicDevice->getDevice());
	m_enemies = std::make_shared<ActorManager<Enemy>>();
	m_bullets = std::make_shared<ActorManager<Bullet>>();
	m_particles = std::make_shared<ActorManager<Particle>>();
	
	m_hudFont = std::make_shared<Font>(20, "Orbitron", false, m_graphicDevice->getDevice());
	m_soundManager->play("bgm", 0.75f, 1.0f, true);
}

bool isHit(Vector2 pos1, Vector2 pos2, float radius1, float radius2) {
	if (std::pow(pos1.x - pos2.x, 2.0f) + std::pow(pos1.y - pos2.y, 2.0f) <= std::pow(radius1 + radius2, 2.0f))
		return true;
	return false;
}

void Play::update() {
	if (m_player->getLife() == 0) {
		m_gameoverCount++;
		if (m_gameoverCount > 180) {
			m_soundManager->stop("bgm");
			changeScene(SceneType::Title);
		}
		return;
	}

	m_player->update();
	m_flail->update();
	m_enemies->update();
	m_bullets->update();
	m_particles->update();

	if (m_viewScore < m_score)
		m_viewScore += (m_score - m_viewScore) / 10 + 1;

	m_frameCount++;
	if (m_frameCount % 1200 == 0) {
		m_level = std::min(m_level + 1, 20);
	}
	if (m_random->next(100 - m_level * 5) == 0 && m_enemies->size() < (1 + m_level)) {
		//プレイヤーから離れた場所に出現させる
		Vector2 pos;
		while (true) {
			pos = Vector2(m_random->next(640.0f), m_random->next(480.0f));
			auto dis = pos - m_player->getPos();
			if (std::abs(dis.length()) > 125)
				break;
		}
		float r = m_random->next(1.0f);
		if (m_level < 2) r -= 0.25f;
		if (m_level < 4) r -= 0.15f;
		if (m_level < 6) r -= 0.05f;
		if (r < 0.75f) {
			auto enemy = std::make_shared<RedEnemy>(pos, m_player, m_particles, m_graphicDevice->getDevice());
			m_enemies->add(enemy);
		} else if (r < 0.85f) {
			auto enemy = std::make_shared<OrangeEnemy>(pos, m_player, m_bullets, m_particles, m_graphicDevice->getDevice());
			m_enemies->add(enemy);
		} else if (r < 0.95f) {
			auto enemy = std::make_shared<PurpleEnemy>(pos, 60.0f, m_player, m_particles, m_graphicDevice->getDevice());
			m_enemies->add(enemy);
		} else {
			auto enemy = std::make_shared<GreenEnemy>(pos, m_player, m_particles, m_graphicDevice->getDevice());
			m_enemies->add(enemy);
		}
	}
	//enemy vs flail
	for (auto& enemy : *m_enemies) {
		if (enemy->canAttack() && isHit(enemy->getPos(), m_flail->getPos(), enemy->getSize()/2, m_flail->getRadius())) {
			auto dis = enemy->getPos() - m_player->getPos();
			auto len = m_flail->getVec().length();
			enemy->blowOff(dis.normalized() * len);
			m_soundManager->play("kin", 0.75f);
			auto sparks = std::make_shared<Sparks>(enemy->getPos(), 15, enemy->getColor(), m_random, m_graphicDevice->getDevice());
			m_particles->add(sparks);
		}
		//hit wall?
		auto pos = enemy->getPos();
		if (pos.x < 0 || pos.x > 640 || pos.y < 0 || pos.y > 480) {
			m_score += enemy->getScore() * m_level;
			enemy->kill();
			auto explosion = std::make_shared<Explosion>(enemy->getPos(), enemy->getColor().setAlpha(1.0f), 75.0f, m_graphicDevice->getDevice());
			m_particles->add(explosion);
			m_soundManager->play("don");
		}
	}
	//enemy vs player
	bool allclean = false;
	for (auto& enemy : *m_enemies) {
		if (m_player->canClash() && isHit(enemy->getPos(), m_player->getPos(), 10.0f, 5.0f)) {
			allclean = true;
			m_player->clash();
			m_soundManager->play("giun");
			break;
		}
	}
	//bullet vs player
	for (auto& bullet : *m_bullets) {
		if (m_player->canClash() && isHit(bullet->getPos(), m_player->getPos(), 5.0f, 5.0f)) {
			allclean = true;
			m_player->clash();
			m_soundManager->play("giun");
			break;
		}
	}
	if (allclean) {
		for (auto& enemy : *m_enemies) {
			enemy->kill();
			auto explosion = std::make_shared<Explosion>(enemy->getPos(), enemy->getColor().setAlpha(1.0f), 75.0f, m_graphicDevice->getDevice());
			m_particles->add(explosion);
		}
		m_bullets->clear();
	}
}

void Play::draw() {
	//draw background
	for (int i = 1; i < 16; i++) {
		Shape::drawRectangle(m_graphicDevice->getDevice(), { i*40.0f-0.5f, 0.0f }, { i*40.0f+0.5f, 480.0f }, Color(0.5f, 0.5f, 1.0f, 0.15f).toD3Dcolor());
		Shape::drawRectangle(m_graphicDevice->getDevice(), { i*40.0f-1.5f, 0.0f }, { i*40.0f+1.5f, 480.0f }, Color(0.5f, 0.5f, 1.0f, 0.075f).toD3Dcolor());
	}
	for (int i = 1; i < 12; i++) {
		Shape::drawRectangle(m_graphicDevice->getDevice(), { 0.0f, i*40.0f-0.5f }, { 640.0f, i*40.0f+0.5f }, Color(0.5f, 0.5f, 1.0f, 0.15f).toD3Dcolor());
		Shape::drawRectangle(m_graphicDevice->getDevice(), { 0.0f, i*40.0f-1.5f }, { 640.0f, i*40.0f+1.5f }, Color(0.5f, 0.5f, 1.0f, 0.075f).toD3Dcolor());
	}

	m_particles->draw();
	m_flail->draw();
	m_player->draw();
	m_enemies->draw();
	m_bullets->draw();

	//draw hud
	m_hudFont->drawStr("SCORE " + std::to_string(m_viewScore), { 10, 10 }, Color(0.6f, 1.0f, 0.6f, 0.9f).toD3Dcolor());
	m_hudFont->drawStr("LIFE", { 10, 30 }, Color(0.5f, 1.0f, 0.5f, 0.9f).toD3Dcolor());
	m_hudFont->drawStr(std::to_string(m_player->getLife()), { 60, 30 }, Color(1.0f, 0.6f, 0.6f, 0.9f).toD3Dcolor());
	m_hudFont->drawStr("LEVEL " + std::to_string(m_level), { 640 - 100, 10 }, Color(0.6f, 1.0f, 0.6f, 0.9f).toD3Dcolor());

	if (m_player->getLife() == 0)
		m_hudFont->drawStr("GAME OVER", { 265, m_gameoverCount < 120 ? (int)Easing::OutBounce(m_gameoverCount, 120, 0, 230) : 230 });
}

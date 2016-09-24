#include "Actors.h"

#include "Input.h"
#include "Graphic.h"
#include "Shape.h"
#include "Util.h"
#include "Easing.h"
#include "Font.h"
#include "Sound.h"
#include "Random.h"

Player::Player(InputMgrPtr inputManager, LPDIRECT3DDEVICE9 d3dDevice) :
m_inputManager(inputManager), m_d3dDevice(d3dDevice),
m_pos(320, 240),
m_stateCount(0), m_mutekiCount(180), m_life(3), m_state(State::Normal)
{}

void Player::update() {
	m_stateCount++; m_mutekiCount--;
	
	switch (m_state) {
	case State::Boot:
		if (m_stateCount == 30) {
			m_stateCount = 0;
			m_mutekiCount = 180;
			m_state = State::Normal;
		}
		break;
	case State::Normal:
		moveCtrl();
		break;
	case State::Damage:
		if (m_stateCount == 60) {
			m_stateCount = 0;
			m_state = State::Boot;
			m_life--;
		}
		break;
	}
}

void Player::moveCtrl() {
	auto dir = m_inputManager->getAxis();
	if (!dir.isZero())
		m_pos += dir.normalized() * 5.0f;
	m_pos = Vector2(clamp(m_pos.x, 0.0f, 640.0f), clamp(m_pos.y, 0.0f, 480.0f));
}

void Player::clash() {
	m_state = State::Damage;
	m_stateCount = 0;
}

void Player::draw() {
	switch (m_state) {
		case State::Boot: {
			float radius = Easing::OutQuint(m_stateCount, 30, 300, 10);
			Color color = HSV(0.7f, Easing::InQuint(m_stateCount, 30, 0.0, 1.0), 1.0f).toColor(Easing::OutQuint(m_stateCount, 30, 0.0, 0.5));
			Shape::drawCircle(m_d3dDevice, m_pos, radius, color.toD3Dcolor());
			break;
		}
		case State::Normal: {
			auto color = Color(0.5f, 0.5f, 1.0f, 0.7f);
			if (m_mutekiCount > 0 && m_mutekiCount % 10 > 5)
				color = Color(0.0f, 0.0f, 0.5f, 0.7f);
			Shape::drawCircle(m_d3dDevice, m_pos, 10.0f, color.toD3Dcolor());
			Shape::drawCircle(m_d3dDevice, m_pos, 5.0f, Color(0.7f, 0.7f, 1.0f, 0.7f).toD3Dcolor());
			break;
		}
		case State::Damage: {
			float alpha = Easing::OutQuint(m_stateCount, 60, 0.7, 0.0);
			Shape::drawCircle(m_d3dDevice, m_pos, 10.0f, Color(0.5f, 0.5f, 1.0f, alpha).toD3Dcolor());
			Shape::drawCircle(m_d3dDevice, m_pos, 5.0f, Color(0.7f, 0.7f, 1.0f, alpha).toD3Dcolor());
			break;
		}
	}
}

Flail::Flail(PlayerPtr player, LPDIRECT3DDEVICE9 d3dDevice) :
m_player(player), m_d3dDevice(d3dDevice),
m_pos(m_player->getPos()), m_vec(), m_radius(30.0f), m_frameCount(0)
{}

void Flail::update() {
	m_frameCount++;

	auto dis = m_player->getPos() - m_pos;
	m_vec += dis / 50;
	m_vec *= 0.975f;
	m_pos += m_vec;

	m_trails.push_front(m_pos);
	if (m_trails.size() > 7)
		m_trails.pop_back();
}

void Flail::draw() {
	auto color = HSV(0.18f - std::min(0.18f/25, 0.18f) * m_vec.length(), 1.0f, 1.0f).toColor(0.6f);
	auto dis = m_player->getPos() - m_pos;
	for (int i = 0; i < 5; i++) {
		Shape::drawCircle(m_d3dDevice, m_pos + dis/5.0f * i, 5.0f, color.toD3Dcolor());
	}

	int cnt = 0;
	for (auto& trail : m_trails) {
		cnt++;
		//Ngon�ł���������
		Shape::drawCircle(m_d3dDevice, trail, m_radius - cnt*1.5f, color.setAlpha(0.6f / cnt).toD3Dcolor());
	}
}

Enemy::Enemy(Vector2 pos, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice),
m_player(player),
m_particles(particles),
m_pos(pos), m_vec(),
m_color(), m_rad(0), m_frameCount(0), m_damageCount(0), m_boot(true)
{}

void Enemy::update() {
	m_frameCount++; m_damageCount--;
	m_rad += 0.05f;
	if (m_boot) {
		if (m_frameCount > 30)
			m_boot = false;
		return;
	}
	if (std::abs(m_vec.length()) > 5.0f && m_frameCount % 3 == 0) {
		auto effect = std::make_shared<Explosion>(m_pos, m_color, 17.5f, m_d3dDevice);
		m_particles->add(effect);
	}
}

void Enemy::draw() {
	if (m_boot) {
		Shape::drawSquare(m_d3dDevice, m_pos, m_size/30 * m_frameCount, m_rad, m_color.toD3Dcolor());
		return;
	}
	Shape::drawSquare(m_d3dDevice, m_pos, m_size/2, m_rad, Color(1.0f, 1.0f, 1.0f, 0.5f).toD3Dcolor());
	Shape::drawSquare(m_d3dDevice, m_pos, m_size, m_rad, m_color.toD3Dcolor());
}

void Enemy::blowOff(Vector2 vec) {
	m_damageCount = 10;
	m_vec += vec;
}

RedEnemy::RedEnemy(Vector2 pos, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, player, particles, d3dDevice)
{
	m_color = Color(1.0f, 0.3f, 0.3f, 0.6f);
	m_size = 30.0f;
	m_score = 100;
}

void RedEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	m_vec *= 0.97f;
	m_pos += Vector2::fromAngle(dis.toAngle()) * 1.5f + m_vec;
}

OrangeEnemy::OrangeEnemy(Vector2 pos, PlayerPtr player, BulletMgrPtr bullets, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, player, particles, d3dDevice), m_bullets(bullets)
{
	m_color = Color(1.0f, 0.5f, 0.0f, 0.6f);
	m_size = 30.0f;
	m_score = 150;
}

void OrangeEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	if (m_frameCount % 120 == 0) {
		auto vec = Vector2::fromAngle(dis.toAngle()) * 3.0f;
		auto bullet = std::make_shared<Bullet>(m_pos, vec, m_d3dDevice);
		m_bullets->add(bullet);
	} else if (m_frameCount % 120 < 90) {
		m_pos += Vector2::fromAngle(dis.toAngle()) * 1.0f;
	} else {
		m_rad += 0.10f;
	}
	m_vec *= 0.97f;
	m_pos += m_vec;
}

GreenEnemy::GreenEnemy(Vector2 pos, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, player, particles, d3dDevice)
{
	m_color = Color(0.5f, 1.0f, 0.5f, 0.5f);
	m_size = 30.0f;
	m_score = 300;
}

void GreenEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	m_vec *= 0.95f;
	m_pos += Vector2::fromAngle(dis.toAngle()) * 2.5f + m_vec;
}

PurpleEnemy::PurpleEnemy(Vector2 pos, float size, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, player, particles, d3dDevice)
{
	m_color = Color(1.0f, 0.3f, 0.8f, 0.5f);
	m_size = size;
	m_score = 200;
}

void PurpleEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	m_vec *= 0.97f;
	m_pos += Vector2::fromAngle(dis.toAngle()) * 1.0f + m_vec;

	if (m_pos.x < 0 || m_pos.x > 640 || m_pos.y < 0 || m_pos.y > 480) {
		if (m_size == 30.0f) return;
		m_size -= 15.0f;
		m_vec *= -0.35f;
		m_pos = Vector2(clamp(m_pos.x, 0.0f, 640.0f), clamp(m_pos.y, 0.0f, 480.0f));
	}
}

Bullet::Bullet(Vector2 pos, Vector2 vec, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice),
m_pos(pos), m_vec(vec), m_frameCount(0)
{}

void Bullet::update() {
	m_frameCount++;
	m_pos += m_vec;
	if (m_pos.x < 0 || m_pos.x > 640 || m_pos.y < 0 || m_pos.y > 480)
		kill();
}

void Bullet::draw() {
	float a = m_frameCount % 10 > 5 ? 0.5f : 0.75f;
	Shape::drawCircle(m_d3dDevice, m_pos, 5.0f, Color(1.0f, 1.0f, 1.0f, a).toD3Dcolor());
}

Particle::Particle(Vector2 pos, LPDIRECT3DDEVICE9 d3dDevice) :
m_pos(pos), m_d3dDevice(d3dDevice), m_frameCount(0)
{}

Explosion::Explosion(Vector2 pos, Color color, float size, LPDIRECT3DDEVICE9 d3dDevice) :
Particle(pos, d3dDevice),
m_color(color), m_size(size), m_alpha(color.a)
{}

void Explosion::update() {
	m_frameCount++;
	if (m_frameCount > 60)
		kill();
}

void Explosion::draw() {
	m_color.a = Easing::OutQuint(m_frameCount, 60, m_alpha, 0.0);
	Shape::drawCircle(m_d3dDevice, m_pos, Easing::OutQuint(m_frameCount, 60, 0.0, m_size), m_color.toD3Dcolor());
}

Sparks::Sparks(Vector2 pos, int num, Color color, RandomPtr random, LPDIRECT3DDEVICE9 d3dDevice) :
Particle(pos, d3dDevice), m_color(color)
{
	for (int i = 0; i < num/2; i++)
		m_sparks.emplace_back(m_pos, Vector2(random->nextPlusMinus(2.0f), random->nextPlusMinus(2.0f)));
	for (int i = 0; i < num/2; i++)
		m_sparks.emplace_back(m_pos, Vector2(random->nextPlusMinus(5.0f), random->nextPlusMinus(5.0f)));
}

void Sparks::update() {
	m_frameCount++;
	if (m_frameCount > 30)
		kill();
	for (auto& spark : m_sparks)
		spark.pos += spark.vec;
}

void Sparks::draw() {
	m_color.a = Easing::Linear(m_frameCount, 30, 1.0f, 0.0f);
	for (auto& spark : m_sparks)
		Shape::drawCircle(m_d3dDevice, spark.pos, 1.5f, m_color.toD3Dcolor());
}

#include "Actors.h"

#include "Input.h"
#include "Graphic.h"
#include "Shape.h"
#include "Util.h"
#include "Easing.h"

Player::Player(std::shared_ptr<InputManager> inputManager, LPDIRECT3DDEVICE9 d3dDevice) :
m_inputManager(inputManager), m_d3dDevice(d3dDevice),
m_pos(320, 240),
m_frameCount(0), m_mutekiCount(0), m_life(3)
{}

void Player::update() {
	m_frameCount++;
	m_mutekiCount--;

	if (!m_inputManager->getXInput()->getLeftThumb().isZero()) {
		m_pos += m_inputManager->getXInput()->getLeftThumb().normalized() * 5.0f;
	} else {
		const Vector2 dir(m_inputManager->isPressedRight() - m_inputManager->isPressedLeft(), m_inputManager->isPressedDown() - m_inputManager->isPressedUp());
		if (!dir.isZero())
			m_pos += dir.normalized() * 5.0f;
	}
	m_pos = Vector2(clamp(m_pos.x, 0.0f, 640.0f), clamp(m_pos.y, 0.0f, 480.0f));
}

void Player::clash() {
	m_mutekiCount = 180;
	m_life--;
}

void Player::init() {
	m_pos = Vector2(320, 240);
	m_frameCount = m_mutekiCount = 0;
	m_life = 3;
}

void Player::draw() {
	auto color = Color(0.5f, 0.5f, 1.0f, 0.5f);
	if (m_mutekiCount > 0 && m_mutekiCount % 10 < 5)
		color = Color(0.0f, 0.0f, 0.5f, 0.5f);
	Shape::drawCircle(m_d3dDevice, m_pos, 10.0f, color.toD3Dcolor());
	Shape::drawCircle(m_d3dDevice, m_pos, 5.0f, Color(0.7f, 0.7f, 1.0f, 0.5f).toD3Dcolor());
}

Flail::Flail(std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice) :
m_player(player), m_d3dDevice(d3dDevice),
m_pos(m_player->getPos()), m_vec(), m_radius(25.0f)
{}

void Flail::update() {
	auto dis = m_player->getPos() - m_pos;
	m_vec += dis / 50;
	m_vec *= 0.975f;
	m_pos += m_vec;

	m_trails.push_front(m_pos);
	if (m_trails.size() > 5)
		m_trails.pop_back();
}

void Flail::draw() {
	Shape::drawLine(m_d3dDevice, m_pos, m_player->getPos(), 1.0f, Color(1.0f, 1.0f, 1.0f, 0.75f).toD3Dcolor());

	int cnt = 0;
	for (auto& trail : m_trails) {
		cnt++;
		Shape::drawCircle(m_d3dDevice, trail, m_radius - cnt, Color(1.0f, 1.0f, 0.4f, 0.6f / cnt).toD3Dcolor());
	}
}

void Flail::init() {
	m_radius = 25.0f;
	m_pos = m_player->getPos();
	m_vec = Vector2();
}

Enemy::Enemy(Vector2 pos, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice),
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
}

void Enemy::draw() {
	if (m_boot) {
		Shape::drawNgon(m_d3dDevice, m_pos, 4, m_size/30 * m_frameCount, m_rad, m_color.toD3Dcolor());
		return;
	}
	Shape::drawNgon(m_d3dDevice, m_pos, 4, m_size/2, m_rad, Color(1.0f, 1.0f, 1.0f, 0.5f).toD3Dcolor());
	Shape::drawNgon(m_d3dDevice, m_pos, 4, m_size, m_rad, m_color.toD3Dcolor());
}

void Enemy::blowOff(Vector2 vec) {
	if (m_damageCount > 0) return;
	m_damageCount = 10;
	m_vec += vec;
}

RedEnemy::RedEnemy(Vector2 pos, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, d3dDevice), m_player(player)
{
	m_color = Color(1.0f, 0.4f, 0.4f, 0.5f);
	m_size = 20.0f;
}

void RedEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	m_vec *= 0.97f;
	m_pos += Vector2::fromAngle(dis.toAngle()) * 1.5f + m_vec;
}

OrangeEnemy::OrangeEnemy(Vector2 pos, std::shared_ptr<Player> player, std::shared_ptr<ActorManager<Bullet>> bullets, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, d3dDevice), m_player(player), m_bullets(bullets)
{
	m_color = Color(1.0f, 0.5f, 0.0f, 0.5f);
	m_size = 20.0f;
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
		m_rad += 0.15f;
	}
	m_vec *= 0.97f;
	m_pos += m_vec;
}

GreenEnemy::GreenEnemy(Vector2 pos, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, d3dDevice), m_player(player)
{
	m_color = Color(0.5f, 1.0f, 0.5f, 0.5f);
	m_size = 20.0f;
}

void GreenEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	m_vec *= 0.95f;
	m_pos += Vector2::fromAngle(dis.toAngle()) * 2.5f + m_vec;
}

PurpleEnemy::PurpleEnemy(Vector2 pos, float size, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice) :
Enemy(pos, d3dDevice), m_player(player)
{
	m_color = Color(1.0f, 0.3f, 0.8f, 0.5f);
	m_size = size;
}

void PurpleEnemy::update() {
	Enemy::update();
	if (m_boot) return;

	auto dis = m_player->getPos() - m_pos;
	m_vec *= 0.97f;
	m_pos += Vector2::fromAngle(dis.toAngle()) * 1.0f + m_vec;

	if (m_pos.x < 0 || m_pos.x > 640 || m_pos.y < 0 || m_pos.y > 480) {
		if (m_size == 20.0f) return;
		m_size -= 10.0f;
		m_vec *= -0.5f;
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
	Shape::drawCircle(m_d3dDevice, m_pos, 5.0f, Color(1.0f, 1.0f, 1.0f, 0.75f).toD3Dcolor());
}

Effect::Effect(Vector2 pos, Color color, float size, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice), m_pos(pos), m_color(color), m_size(size), m_frameCount(0)
{}

void Effect::update() {
	m_frameCount++;
	if (m_frameCount > 60)
		kill();
}

void Effect::draw() {
	m_color.a = Easing::OutQuint(m_frameCount, 60, 1.0, 0.0);
	Shape::drawCircle(m_d3dDevice, m_pos, Easing::OutQuint(m_frameCount, 60, 0.0, m_size), m_color.toD3Dcolor());
}

#include "Actors.h"

#include "Input.h"
#include "Graphic.h"
#include "Shape.h"
#include "Util.h"

Player::Player(std::shared_ptr<InputManager> inputManager, LPDIRECT3DDEVICE9 d3dDevice) :
m_inputManager(inputManager), m_d3dDevice(d3dDevice),
m_pos(320, 240), m_frameCount(0)
{}

void Player::update() {
	m_frameCount++;

	const Vector2 dir(m_inputManager->isPressedRight() - m_inputManager->isPressedLeft(), m_inputManager->isPressedDown() - m_inputManager->isPressedUp());
	if (!dir.isZero())
		m_pos += dir.normalized() * 5.0f;

	m_pos = Vector2(clamp(m_pos.x, 0.0f, 640.0f), clamp(m_pos.y, 0.0f, 480.0f));
}

void Player::draw() {
	Shape::drawCircle(m_d3dDevice, m_pos, 10.0f, D3DCOLOR_ARGB(122, 150, 150, 255));
}

Flail::Flail(std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice) :
m_player(player), m_d3dDevice(d3dDevice),
m_pos(m_player->getPos()), m_vec(), m_radius(25.0f)
{}

void Flail::update() {
	auto dis = m_player->getPos() - m_pos;
	m_vec += dis / 60;
	m_vec *= 0.98f;
	m_pos += m_vec;

	m_radius = dis.length() / 5.0f + 5;

	m_trails.push_front(m_pos);
	if (m_trails.size() > 3)
		m_trails.pop_back();
}

void Flail::draw() {
	Shape::drawLine(m_d3dDevice, m_pos, m_player->getPos());

	int cnt = 0;
	for (auto& trail : m_trails) {
		cnt++;
		Shape::drawCircle(m_d3dDevice, trail, m_radius, D3DCOLOR_ARGB(122/cnt, 255, 255, 100));
	}
}

Enemy::Enemy(Vector2 pos, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice) :
m_player(player), m_d3dDevice(d3dDevice),
m_pos(pos), m_rad(0)
{}

void Enemy::update() {
	auto dis = m_player->getPos() - m_pos;
	m_pos += Vector2::fromAngle(std::atan2(dis.y, dis.x)) * 1.5f;
	m_rad += 0.1f;
}

void Enemy::draw() {
	Shape::drawNgon(m_d3dDevice, m_pos, 6, 20.0f, m_rad, D3DCOLOR_ARGB(122, 255, 100, 100));
}

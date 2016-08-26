#include "Actors.h"

#include "Input.h"
#include "Graphic.h"
#include "Shape.h"

Player::Player(TSGame* game, std::shared_ptr<InputManager> inputManager, LPDIRECT3DDEVICE9 d3dDevice) :
m_game(game), m_inputManager(inputManager), m_d3dDevice(d3dDevice),
m_pos(320, 240), m_frameCount(0)
{}

void Player::update() {
	m_frameCount++;

	const Vector2 dir(m_inputManager->isPressedRight() - m_inputManager->isPressedLeft(), m_inputManager->isPressedDown() - m_inputManager->isPressedUp());
	if (!dir.isZero())
		m_pos += dir.normalized() * 5.0f;
}

void Player::draw() {
	Shape::drawCircle(m_d3dDevice, m_pos, 10.0f, D3DCOLOR_ARGB(122, 150, 150, 255));
}

Enemy::Enemy(Vector2 pos, TSGame* game, LPDIRECT3DDEVICE9 d3dDevice) :
m_game(game), m_d3dDevice(d3dDevice),
m_pos(pos), m_rad(0)
{}

void Enemy::update() {
	auto dis = m_game->getPlayer()->getPos() - m_pos;
	m_pos += Vector2::fromAngle(std::atan2(dis.y, dis.x)) * 1.5f;
	m_rad += 0.1f;
}

void Enemy::draw() {
	Shape::drawNgon(m_d3dDevice, m_pos, 6, 20.0f, m_rad, D3DCOLOR_ARGB(122, 255, 100, 100));
}

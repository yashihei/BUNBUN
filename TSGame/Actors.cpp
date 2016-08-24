#include "Actors.h"

#include "Input.h"
#include "Graphic.h"
#include "Shape.h"

Player::Player(TSGame* game, std::shared_ptr<InputManager> inputManager) :
m_game(game), m_inputManager(m_inputManager),
m_pos(320, 240), m_frameCount(0)
{}

void Player::update() {
	m_frameCount++;
}

void Player::draw() {
	Shape::drawCircle(m_pos, 10, D3DCOLOR_ARGB(122, 150, 150, 255));
}

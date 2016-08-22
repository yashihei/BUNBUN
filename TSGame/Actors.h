#pragma once
#include <d3dx9.h>
#include "Actor.h"
#include "TSGame.h"
#include "Input.h"

class Player {
public:
	Player(TSGame* game, std::shared_ptr<InputManager> inputManager, std::shared_ptr<TextureManager> textureManager) :
		m_game(game), m_inputManager(m_inputManager), m_textureManager(textureManager),
		m_pos(320, 240), m_vec(0, 0), m_frameCount(0)
	{}
	void update() {
		m_frameCount++;
	}
	void draw() {
	}
private:
	TSGame* m_game;
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<TextureManager> m_textureManager;
	D3DXVECTOR2 m_pos, m_vec;
	int m_frameCount;
};

class Shot : public Actor {
};

class Enemy : public Actor {
};

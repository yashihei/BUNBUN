#pragma once
#include <d3dx9.h>
#include "TSGame.h"
#include "Actor.h"
#include "Vector2.h"

class Player {
public:
	Player(TSGame* game, std::shared_ptr<InputManager> inputManager);
	void update();
	void draw();
private:
	TSGame* m_game;
	std::shared_ptr<InputManager> m_inputManager;
	Vector2 m_pos;
	int m_frameCount;
};

class Enemy {
public:
private:
};

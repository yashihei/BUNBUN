#pragma once
#include <d3dx9.h>
#include "TSGame.h"
#include "Actor.h"
#include "Vector2.h"

class Player {
public:
	Player(TSGame* game, std::shared_ptr<InputManager> inputManager, LPDIRECT3DDEVICE9 d3dDevice);
	void update();
	void draw();
	Vector2 getPos() const { return m_pos; }
private:
	TSGame* m_game;
	std::shared_ptr<InputManager> m_inputManager;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	int m_frameCount;
};

class Enemy : public Actor {
public:
	Enemy(Vector2 pos, TSGame* game, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
private:
	TSGame* m_game;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	float m_rad;
};

#pragma once
#include <d3dx9.h>
#include <deque>
#include "TSGame.h"
#include "Actor.h"
#include "Vector2.h"

class Player {
public:
	Player(std::shared_ptr<InputManager> inputManager, LPDIRECT3DDEVICE9 d3dDevice);
	void update();
	void draw();
	void clash();
	bool muteki() const { return m_mutekiCount > 0 ? true : false; }
	Vector2 getPos() const { return m_pos; }
	int getLife() const { return m_life; }
private:
	std::shared_ptr<InputManager> m_inputManager;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	int m_frameCount, m_mutekiCount, m_life;
};

class Flail {
public:
	Flail(std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice);
	void update();
	void draw();
	Vector2 getPos() const { return m_pos; }
	float getRadius() const { return m_radius; }
private:
	std::shared_ptr<Player> m_player;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos, m_vec;
	float m_radius;
	std::deque<Vector2> m_trails;
};

class Enemy : public Actor {
public:
	Enemy(Vector2 pos, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
	Vector2 getPos() const { return m_pos; }
	bool start() const { return m_start; }
private:
	std::shared_ptr<Player> m_player;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	float m_rad;
	int m_frameCount;
	bool m_start;
};

class Bullet : public Actor {
public:
	Bullet(Vector2 pos, Vector2 vec, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
	Vector2 getPos() const { return m_pos; }
private:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos, m_vec;
	int m_frameCount;
};

class Effect : public Actor {
public:
	Effect(Vector2 pos, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
private:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	int m_frameCount;
};

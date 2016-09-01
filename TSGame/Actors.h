#pragma once
#include <d3dx9.h>
#include <deque>
#include "TSGame.h"
#include "Actor.h"
#include "Vector2.h"
#include "Color.h"

class Player {
public:
	Player(std::shared_ptr<InputManager> inputManager, LPDIRECT3DDEVICE9 d3dDevice);
	void update();
	void draw();
	void clash();
	void init();
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
	void init();
	Vector2 getPos() const { return m_pos; }
	Vector2 getVec() const { return m_vec; }
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
	Enemy(Vector2 pos, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
	void blowOff(Vector2 vec);
	Vector2 getPos() const { return m_pos; }
	Color getColor() const { return m_color; }
	float getSize() const { return m_size; }
	bool isBooting() const { return m_boot; }
protected:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos, m_vec;
	Color m_color;
	float m_rad, m_size;
	int m_frameCount, m_damageCount;
	bool m_boot;
};

class RedEnemy : public Enemy {
public:
	RedEnemy(Vector2 pos, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
private:
	std::shared_ptr<Player> m_player;
};

class Bullet;
class OrangeEnemy : public Enemy {
public:
	OrangeEnemy(Vector2 pos, std::shared_ptr<Player> player, std::shared_ptr<ActorManager<Bullet>> bullets, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
private:
	std::shared_ptr<Player> m_player;
	std::shared_ptr<ActorManager<Bullet>> m_bullets;
};

class GreenEnemy : public Enemy {
public:
	GreenEnemy(Vector2 pos, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
private:
	std::shared_ptr<Player> m_player;
};

class PurpleEnemy : public Enemy {
public:
	PurpleEnemy(Vector2 pos, float size, std::shared_ptr<Player> player, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
private:
	std::shared_ptr<Player> m_player;
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
	Effect(Vector2 pos, Color color, float size, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
private:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	Color m_color;
	float m_size;
	int m_frameCount;
};

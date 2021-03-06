#pragma once
#define NOMINMAX
#include <d3dx9.h>
#include <deque>
#include <vector>
#include "Actor.h"
#include "Vector2.h"
#include "Color.h"
#include "Fwd.h"

class Player {
public:
	Player(InputMgrPtr inputManager, LPDIRECT3DDEVICE9 d3dDevice);
	void update();
	void draw();
	void moveCtrl();
	void clash();
	bool canClash() const { return m_mutekiCount > 0 || m_state != State::Normal ? false : true; }
	Vector2 getPos() const { return m_pos; }
	int getLife() const { return m_life; }
private:
	InputMgrPtr m_inputManager;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	int m_stateCount, m_mutekiCount, m_life;
	enum class State {
		Boot, Normal, Damage
	} m_state;
};

class Flail {
public:
	Flail(PlayerPtr player, LPDIRECT3DDEVICE9 d3dDevice);
	void update();
	void draw();
	Vector2 getPos() const { return m_pos; }
	Vector2 getVec() const { return m_vec; }
	float getRadius() const { return m_radius; }
private:
	PlayerPtr m_player;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos, m_vec;
	float m_radius;
	int m_frameCount;
	std::deque<Vector2> m_trails;
};

class Enemy : public Actor {
public:
	Enemy(Vector2 pos, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
	void blowOff(Vector2 vec);
	Vector2 getPos() const { return m_pos; }
	Color getColor() const { return m_color; }
	float getSize() const { return m_size; }
	int getScore() const { return m_score; }
	bool canAttack() const { return m_boot || (m_damageCount > 0) ? false : true; }
protected:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	PlayerPtr m_player;
	ParticleMgrPtr m_particles;
	Vector2 m_pos, m_vec;
	Color m_color;
	float m_rad, m_size;
	int m_frameCount, m_damageCount, m_score;
	bool m_boot;
};

class RedEnemy : public Enemy {
public:
	RedEnemy(Vector2 pos, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
};

class Bullet;
class OrangeEnemy : public Enemy {
public:
	OrangeEnemy(Vector2 pos, PlayerPtr player, BulletMgrPtr bullets, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
private:
	BulletMgrPtr m_bullets;
};

class GreenEnemy : public Enemy {
public:
	GreenEnemy(Vector2 pos, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
};

class PurpleEnemy : public Enemy {
public:
	PurpleEnemy(Vector2 pos, float size, PlayerPtr player, ParticleMgrPtr particles, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
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

class Particle : public Actor {
public:
	Particle(Vector2 pos, LPDIRECT3DDEVICE9 d3dDevice);
protected:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	Vector2 m_pos;
	int m_frameCount;
};

class Explosion : public Particle {
public:
	Explosion(Vector2 pos, Color color, float size, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
private:
	Color m_color;
	float m_size, m_alpha;
};

class Sparks : public Particle {
public:
	Sparks(Vector2 pos, int num, Color color, RandomPtr random, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;
private:
	struct Spark {
		Spark(Vector2 pos_, Vector2 vec_) : pos(pos_), vec(vec_) {}
		Vector2 pos, vec;
	};
	std::vector<Spark> m_sparks;
	Color m_color;
};

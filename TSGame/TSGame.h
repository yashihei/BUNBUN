#pragma once
#define NOMINMAX
#include <memory>
#include <deque>
#include <d3dx9.h>
#include "Actor.h"
#include "GameApp.h"

class Player;
class Flail;
class Enemy;
class Bullet;
class Effect;
class Font;

using PlayerPtr = std::shared_ptr<Player>;
using FlailPtr = std::shared_ptr<Flail>;
using EnemyMgrPtr = std::shared_ptr<ActorManager<Enemy>>;
using BulletMgrPtr = std::shared_ptr<ActorManager<Bullet>>;
using EffectMgrPtr = std::shared_ptr<ActorManager<Effect>>;

class TSGame : public GameApp {
public:
	TSGame(HWND hWnd, HINSTANCE hInstance);
private:
	void update() override;
	void draw() override;

	PlayerPtr m_player;
	FlailPtr m_flail;
	EnemyMgrPtr m_enemies;
	BulletMgrPtr m_bullets;
	EffectMgrPtr m_effects;
	std::shared_ptr<Font> m_hudFont, m_titleFont;
	int m_score, m_level;
	bool m_gameover;
};

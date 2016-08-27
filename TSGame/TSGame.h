#pragma once
#include <memory>
#include <d3dx9.h>
#include "Actor.h"
#include "GameApp.h"

class Player;
class Flail;
class Enemy;
class Font;

class TSGame : public GameApp {
public:
	TSGame(HWND hWnd, HINSTANCE hInstance);
private:
	void update() override;
	void draw() override;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<Flail> m_flail;
	std::shared_ptr<ActorManager<Enemy>> m_enemies;
	std::shared_ptr<Font> m_hudFont;
	int m_score;
};

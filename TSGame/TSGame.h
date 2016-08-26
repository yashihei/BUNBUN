#pragma once
#include <memory>
#include <d3dx9.h>
#include "Actor.h"
#include "GameApp.h"

class Player;
class Enemy;

class TSGame : public GameApp {
public:
	TSGame(HWND hWnd, HINSTANCE hInstance);
	std::shared_ptr<Player> getPlayer() { return m_player; }
private:
	void update() override;
	void draw() override;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<ActorManager<Enemy>> m_enemies;
};

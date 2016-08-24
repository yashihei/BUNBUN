#pragma once
#include <memory>
#include <d3dx9.h>
#include "GameApp.h"

class Player;

class TSGame : public GameApp {
public:
	TSGame(HWND hWnd, HINSTANCE hInstance);
private:
	void update() override;
	void draw() override;
	std::shared_ptr<Player> m_player;
};

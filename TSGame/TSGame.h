#pragma once
#include <memory>
#include <d3dx9.h>
#include "GameApp.h"

class Animation;

class TSGame : public GameApp {
public:
	TSGame(HWND hWnd, HINSTANCE hInstance);
private:
	void update() override;
	void draw() override;
};

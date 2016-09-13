#pragma once
#define NOMINMAX
#include <d3dx9.h>
#include <memory>
#include "GameApp.h"
#include "Scene.h"

class TSGame : public GameApp {
public:
	TSGame(HWND hWnd, HINSTANCE hInstance);
	void update() override;
	void draw() override;
private:
	void changeChene(SceneType type);
	std::shared_ptr<Scene> m_scene;
};

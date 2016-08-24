#include "TSGame.h"

#include "Graphic.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"
#include "Util.h"
#include "Font.h"
#include "Actors.h"

TSGame::TSGame(HWND hWnd, HINSTANCE hInstance) :
GameApp(hWnd, hInstance) 
{
	//m_textureManager->load("dat/player.png", "player");
	m_player = std::make_shared<Player>(this, m_inputManager, m_graphicDevice->getDevice());
}

void TSGame::update() {
	m_player->update();
}

void TSGame::draw() {
	m_player->draw();
}

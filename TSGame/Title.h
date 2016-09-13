#pragma once
#include "Scene.h"
#include "Fwd.h"

#include "Graphic.h"
#include "Font.h"
#include "Color.h"
#include "Input.h"

class Title : public Scene {
public:
	Title(GraphicDevicePtr graphic, SoundMgrPtr soundManager, InputMgrPtr inputManager) :
	m_graphicDevice(graphic), m_soundManager(soundManager), m_inputManager(inputManager),
	m_frameCount(0)
	{
		m_titleFont = std::make_shared<Font>(50, "Orbitron", false, m_graphicDevice->getDevice());
		m_textFont = std::make_shared<Font>(20, "Orbitron", false, m_graphicDevice->getDevice());
	}
	void update() override {
		m_frameCount++;
		if (m_inputManager->isClickedButton1())
			changeScene(SceneType::Play);
	}
	void draw() override {
		m_titleFont->drawStr("BUN BUN", { 10, 200 }, Color(1.0f, 0.5f, 0.0f, 0.8f).toD3Dcolor());
		if (m_frameCount % 60 < 30) {
			if (m_inputManager->getXInput()->isConnecting())
				m_textFont->drawStr("PUSH BUTTON 1 TO START", { 10, 250 }, Color().toD3Dcolor());
			else
				m_textFont->drawStr("PUSH Z TO START", { 10, 250 }, Color().toD3Dcolor());
		}
	}
private:
	GraphicDevicePtr m_graphicDevice;
	SoundMgrPtr m_soundManager;
	InputMgrPtr m_inputManager;

	int m_frameCount;
	FontPtr m_titleFont, m_textFont;
};

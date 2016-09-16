#pragma once
#include "Scene.h"
#include "Fwd.h"

#include "Graphic.h"
#include "Font.h"
#include "Color.h"
#include "Input.h"
#include "Util.h"
#include "Shape.h"

class Title : public Scene {
public:
	Title(GraphicDevicePtr graphic, SoundMgrPtr soundManager, InputMgrPtr inputManager) :
	m_graphicDevice(graphic), m_soundManager(soundManager), m_inputManager(inputManager),
	m_frameCount(0), m_changeCount(0), m_select(0), m_changeFlag(false)
	{
		m_titleFont = std::make_shared<Font>(50, "Orbitron", false, m_graphicDevice->getDevice());
		m_textFont = std::make_shared<Font>(20, "Orbitron", false, m_graphicDevice->getDevice());
	}
	void update() override {
		m_frameCount++;

		if (m_changeFlag) {
			m_changeCount++;
			if (m_changeCount > 60)
				changeScene(SceneType::Play);
			return;
		}

		if (m_inputManager->isClickedButton() && m_select == 0)
			m_changeFlag = true;
		if (m_inputManager->isClickedButton() && m_select == 3)
			PostQuitMessage(0);
		//select
		if (m_inputManager->isClickedUp())
			m_select = wrap(m_select - 1, 0, 4);
		if (m_inputManager->isClickedDown())
			m_select = wrap(m_select + 1, 0, 4);
	}
	void draw() override {
		//background
		m_titleFont->drawStr("BUN BUN", { 10, 200 }, Color(1.0f, 0.5f, 0.0f, 1.0f).toD3Dcolor());

		Shape::drawRectangle(m_graphicDevice->getDevice(), {0.0f, 250 + m_select*20.0f}, {640.0f, 270 + m_select*20.0f}, Color(1.0f, 1.0f, 1.0f, 1.0f).toD3Dcolor());
		const std::vector<std::string> texts = { "START GAME", "HOW TO PLAY", "RANKING", "EXIT" };
		for (int i = 0; i < texts.size(); i++) {
			auto color = (i == m_select) ? Color(0.0f, 0.0f, 0.0f, std::abs(std::sin(D3DX_PI/30*m_frameCount))) : Color(1.0f, 1.0f, 1.0f, 1.0f);
			m_textFont->drawStr(texts[i], { 10, 250 + (i * 20) }, color.toD3Dcolor());
		}
		//fadeout
		Shape::drawRectangle(m_graphicDevice->getDevice(), { 0.0f, 0.0f }, { 640.0f, 480.0f }, Color(0.0f, 0.0f, 0.0f, 1.0f / 60 * m_changeCount).toD3Dcolor());
	}
private:
	GraphicDevicePtr m_graphicDevice;
	SoundMgrPtr m_soundManager;
	InputMgrPtr m_inputManager;

	int m_frameCount, m_changeCount, m_select;
	bool m_changeFlag;
	FontPtr m_titleFont, m_textFont;
};

#pragma once
#define NOMINMAX
#include <memory>
#include <deque>
#include <d3dx9.h>
#include "Scene.h"
#include "Fwd.h"

class Play : public Scene {
public:
	Play(GraphicDevicePtr graphic, SoundMgrPtr soundManager, InputMgrPtr inputManager, RandomPtr random);
	void update() override;
	void draw() override;
private:
	GraphicDevicePtr m_graphicDevice;
	SoundMgrPtr m_soundManager;
	InputMgrPtr m_inputManager;
	RandomPtr m_random;

	PlayerPtr m_player;
	FlailPtr m_flail;
	EnemyMgrPtr m_enemies;
	BulletMgrPtr m_bullets;
	EffectMgrPtr m_effects;
	FontPtr m_hudFont;
	int m_frameCount, m_gameoverCount, m_score, m_viewScore, m_level;
};

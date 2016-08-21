#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <d3dx9.h>
#include "Rect.h"

class Texture;

class Animation {
public:
	Animation(std::shared_ptr<Texture> texture, int col, int row, int interval);
	void update();
	void addPattern(std::string alias, int startFrame, int endFrame);
	void addPattern(std::string alias, std::vector<int> patternList) { m_patterns[alias] = patternList; }
	void changePattern(std::string alias) { m_currentPattern = alias; m_cnt1 = m_cnt2 = 0; }
	void setInterval(int interval) { m_interval = interval; }
	bool isPlaying(std::string alias) { return alias == m_currentPattern; }
	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
private:
	void setRect();
	std::shared_ptr<Texture> m_texture;
	int m_col, m_row, m_interval, m_cnt1, m_cnt2;
	RectF m_uvRect;
	std::string m_currentPattern;
	std::unordered_map<std::string, std::vector<int>> m_patterns;
};

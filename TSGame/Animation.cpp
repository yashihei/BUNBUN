#include "Animation.h"

#include "Texture.h"

Animation::Animation(std::shared_ptr<Texture> texture, int col, int row, int interval) :
m_texture(texture),
m_col(col), m_row(row), m_interval(interval), m_cnt1(0), m_cnt2(0),
m_uvRect(0.0f, 0.0f, 1.0f / m_col, 1.0f / m_row)
{}

void Animation::update() {
	if (m_cnt1 % m_interval == 0)
		m_cnt2++;
	m_cnt1++;
	setRect();
}

void Animation::setRect() {
	int index = 0;
	if (m_currentPattern.empty()) {
		index = m_cnt2 % (m_col * m_row);
	} else {
		int patternLen = m_patterns[m_currentPattern].size();
		index = m_patterns[m_currentPattern][m_cnt2 % patternLen];
	}
	m_uvRect.x = m_uvRect.w * (index % m_col);
	m_uvRect.y = m_uvRect.h * (index / m_col);
}

void Animation::addPattern(std::string alias, int startFrame, int endFrame) {
	std::vector<int> patternList;
	for (int i = startFrame; i < endFrame; i++)
		patternList.push_back(i);
	m_patterns[alias] = patternList;
}

void Animation::draw(Vector2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	m_texture->draw(m_uvRect, pos, rad, scale, color, mirror);
}

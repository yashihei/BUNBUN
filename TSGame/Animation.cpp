#include "Animation.h"

#include "Texture.h"

Animation::Animation(std::shared_ptr<Texture> texture, int col, int row, int interval) :
m_texture(texture),
m_col(col), m_row(row), m_interval(interval), m_cnt(0), m_relativeFrame(0), m_absFrame(0)
{}

void Animation::update() {
	m_cnt++;
	if (m_cnt % m_interval == 0)
		m_relativeFrame++;

	if (m_currentPattern.empty()) {
		m_absFrame = m_relativeFrame % (m_col * m_row);
	} else {
		int patternLen = m_patterns[m_currentPattern].size();
		m_absFrame = m_patterns[m_currentPattern][m_relativeFrame % patternLen];
	}
}

void Animation::addPattern(std::string alias, int startFrame, int endFrame) {
	std::vector<int> patternList;
	for (int i = startFrame; i < endFrame; i++)
		patternList.push_back(i);
	m_patterns[alias] = patternList;
}

void Animation::draw(D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	m_texture->drawFrame(m_col, m_row, m_absFrame, pos, rad, scale, color, mirror);
}

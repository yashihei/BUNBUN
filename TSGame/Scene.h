#pragma once

enum class SceneType {
	Title, Play, Result, None
};

class Scene {
public:
	virtual ~Scene() = default;
	virtual void update() = 0;
	virtual void draw() = 0;
	void moveTo(SceneType type) { m_next = type; }
	virtual SceneType nextScene() { return m_next; }
private:
	SceneType m_next = SceneType::None;
};

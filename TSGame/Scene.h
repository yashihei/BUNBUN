#pragma once

enum class SceneType {
	Title, Play, Result, None
};

class Scene {
public:
	virtual ~Scene() = default;
	virtual SceneType update() = 0;
};

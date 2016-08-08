#pragma once

#include <memory>
#include <list>

class Actor {
public:
	Actor() : m_enable(true) {}
	virtual ~Actor() = default;
	virtual void update() = 0;
	virtual void draw() = 0;

	void kill() { m_enable = false; }
	bool isEnabled() const { return m_enable; }
private:
	bool m_enable;
};

template<typename Type>
class ActorManager {
public:
	ActorManager() = default;
	virtual ~ActorManager() = default;

	void add(std::shared_ptr<Type> actor) {
		m_actors.push_back(actor);
	}
	void clear() {
		m_actors.clear();
	}
	int size() {
		return m_actors.size();
	}
	void update() {
		for (auto& actor : m_actors) {
			actor->update();
		}
		m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(),
			[](std::shared_ptr<Type> actor) { return !actor->isEnabled(); }), m_actors.end());
	}
	void draw() {
		for (auto& actor : m_actors) {
			actor->draw();
		}
	}

	typename std::list<std::shared_ptr<Type>>::const_iterator begin() { return m_actors.begin(); }
	typename std::list<std::shared_ptr<Type>>::const_iterator end() { return m_actors.end(); }
private:
	std::list<std::shared_ptr<Type>> m_actors;
};

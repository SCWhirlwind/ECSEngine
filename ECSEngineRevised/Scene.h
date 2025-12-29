#pragma once

#include "Action.hpp"
#include "EntityManager.h"
#include "Graphics.h"
#include "Vec2.hpp"
#include <memory>
#include "InputManager.h"

class GameManager;

using ActionMap = std::map<int, std::string>;

class Scene
{
public:
	Scene() = default;
	Scene(GameManager* game,
		Graphics* graphics = &Graphics::Instance(),
		InputManager* inputManager = &InputManager::Instance());

	virtual ~Scene() = default;

	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	virtual void doAction(const Action& action);

	void simulate(const size_t frame);
	void registerAction(int inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	bool isPaused() const { return m_paused; }

	void end() { if (!m_hasEnded) m_hasEnded = true; onEnd(); }

	bool hasEnded() const;
	const ActionMap& getActionMap() const;

protected:

	GameManager* m_game = nullptr;
	Graphics* m_graphics = nullptr;
	InputManager* m_inputManager = nullptr;
	EntityManager m_entityManager;

	ActionMap m_actionMap;
	bool m_paused = false;
	bool m_hasEnded = false;
	size_t m_currentFrame = 0;

	virtual void onEnd() = 0;
	void setPaused(bool paused);
};


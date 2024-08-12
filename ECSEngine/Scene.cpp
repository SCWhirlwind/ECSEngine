#include "Scene.h"

Scene::Scene(GameManager* game) : m_game(game)
{
	m_graphics = Graphics::Instance();
	m_inputManager = InputManager::Instance();
}

Scene::~Scene()
{
	m_game = nullptr;
	m_graphics = nullptr;
}


void Scene::doAction(const Action& action)
{
	sDoAction(action);
}

void Scene::simulate(const size_t frame)
{
	for (size_t i = 0; i < frame; i++)
	{
		update();
	}
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

size_t Scene::width() const
{
	return m_graphics->SCREEN_WIDTH;
}

size_t Scene::height() const
{
	return m_graphics->SCREEN_HEIGHT;
}

size_t Scene::currentFrame() const
{
	return m_currentFrame;
}

bool Scene::hasEnded() const
{
	return m_hasEnded;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
}

void Scene::setPaused(bool paused)
{
	m_paused = paused;
}

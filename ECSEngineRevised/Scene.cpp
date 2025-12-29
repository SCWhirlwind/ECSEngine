#include "Scene.h"

Scene::Scene(GameManager* game, Graphics* graphics, InputManager* inputManager) 
	: m_game(game),
	m_graphics(graphics),
	m_inputManager(inputManager)
{}

void Scene::doAction(const Action& action)
{
	sDoAction(action);
}

void Scene::simulate(const size_t frame)
{
	for (size_t i = 0; i < frame; i++)
	{
		update();
		m_currentFrame++;
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

void Scene::setPaused(bool paused)
{
	m_paused = paused;
}

#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"
#include "SceneHeaders.h"
#include <iostream>

Scene_Menu::Scene_Menu()
{
}

Scene_Menu::Scene_Menu(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Menu::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "TEST")
		{
			std::cout << "Registered Key!" << std::endl;
		}
		else if (action.getName() == "QUIT")
		{
			onEnd();
		}
	}
	if (action.getType() == "END")
	{
	}
}

void Scene_Menu::sRender()
{
	for (auto& e : m_entityManager.getEntities())
	{

		if (e->hasComponent<SpriteComponent>())
		{
			auto& sprite = e->getComponent<SpriteComponent>();
			auto& transform = e->getComponent<TransformComponent>();

			sprite.destRect.x = static_cast<int>(transform.position.x);
			sprite.destRect.y = static_cast<int>(transform.position.y);
			sprite.destRect.w = sprite.width * sprite.scale;
			sprite.destRect.h = sprite.height * sprite.scale;
			m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
		}
	}
}

void Scene_Menu::update()
{
	m_entityManager.update();
}

void Scene_Menu::init()
{
	m_title = "MENU";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");

	m_entityManager = EntityManager();


}

void Scene_Menu::onEnd()
{
	m_game->Quit();
}


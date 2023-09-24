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
			std::cout << "Registerd Key!" << std::endl;
		}
		else if (action.getName() == "UP")
		{
			if (m_menuIndex == 0)
			{
				m_menuIndex = 4;
			}
			else
			{
				m_menuIndex--;
			}
			moveCursor(m_menuIndex);
		}
		else if (action.getName() == "DOWN")
		{
			if (m_menuIndex == 4)
			{
				m_menuIndex = 0;
			}
			else
			{
				m_menuIndex++;
			}
			moveCursor(m_menuIndex);
		}
		else if (action.getName() == "SELECT")
		{
			switch (m_menuIndex)
			{
				case 0:
				{
					m_game->changeScene("Grid", std::make_shared<Scene_Grid>(m_game));
					break;
				}
				case 1:
				{
					m_game->changeScene("Camera", std::make_shared<Scene_Camera>(m_game));
					break;
				}
				case 2:
				{
					m_game->changeScene("Spawn", std::make_shared<Scene_Spawn>(m_game));
					break;
				}
				case 3:
				{
					m_game->changeScene("Platformer", std::make_shared<Scene_Platformer>(m_game));
					break;
				}
				case 4:
				{
					m_game->changeScene("Editor", std::make_shared<Scene_Editor>(m_game));
					break;
				}
			}
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
	for (auto &e : m_entityManager.getEntities())
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
		else if (e->hasComponent<TextComponent>())
		{
			auto& sprite = e->getComponent<TextComponent>();
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
	registerAction(SDL_SCANCODE_RETURN, "SELECT");
	registerAction(SDL_SCANCODE_W, "UP");
	registerAction(SDL_SCANCODE_S, "DOWN");

	m_entityManager = EntityManager();
	
	createText("Menu", 100, Vec2(100, 75));
	createText("Grid Example", 50, Vec2(100, 300));
	createText("Camera Example", 50, Vec2(100, 350));
	createText("Entity Creation and Deletion Example", 25, Vec2(100, 400));
	createText("Platformer Example", 40, Vec2(100, 450));
	createText("Editor Example", 40, Vec2(100, 500));
	createCursor();
}

void Scene_Menu::onEnd()
{
	m_game->Quit();
}

void Scene_Menu::createText(std::string s, int size, Vec2 pos)
{
	auto text = m_entityManager.addEntity("Text");
	text->addComponent<TextComponent>(s, "Snes.ttf", size, 300, 200, 1, SDL_Color { 255, 255, 255 });
	text->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
}

void Scene_Menu::createCursor()
{
	cursor = m_entityManager.addEntity("Cursor");
	cursor->addComponent<SpriteComponent>("cursor.png", 32, 32, 1);
	cursor->addComponent<TransformComponent>(Vec2(350, 300), Vec2(0, 0), 0);
}

void Scene_Menu::moveCursor(int i)
{
	cursor->getComponent<TransformComponent>().position.y = 300 + 50 * i;
}




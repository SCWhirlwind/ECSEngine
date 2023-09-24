#include "Scene_Editor.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"


Scene_Editor::Scene_Editor()
{
}

Scene_Editor::Scene_Editor(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Editor::sDoAction(const Action& action)
{
	if (!isInput)
	{
		if (action.getType() == "START")
		{
			if (action.getName() == "QUIT")
			{
				onEnd();
			}
		}
		if (state == MENU)
		{
			if (action.getType() == "START")
			{
				if (action.getName() == "UP")
				{
					if (m_menuIndex == 0)
					{
						m_menuIndex = 1;
					}
					else
					{
						m_menuIndex--;
					}
					moveCursor(m_menuIndex);
				}
				if (action.getName() == "DOWN")
				{
					if (m_menuIndex == 1)
					{
						m_menuIndex = 0;
					}
					else
					{
						m_menuIndex++;
					}
					moveCursor(m_menuIndex);
				}
				if (action.getName() == "LEFT")
				{
				}
				if (action.getName() == "RIGHT")
				{
				}
				if (action.getName() == "SELECT")
				{
					switch (m_menuIndex)
					{
					case 0:
					{
						state = NEW;
						break;
					}
					case 1:
					{
						state = LOAD;
						break;
					}
					default:
						break;
					}
				}
			}
			if (action.getType() == "END")
			{
				if (action.getName() == "UP")
				{
				}
				if (action.getName() == "DOWN")
				{
				}
				if (action.getName() == "LEFT")
				{
				}
				if (action.getName() == "RIGHT")
				{
				}
			}
		}
	}
}

void Scene_Editor::sRender()
{
	if (state == MENU)
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
	if (state == NEW)
	{
		for (auto& e : m_entityManager.getEntities("SubMenu"))
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
				if (m_game->getRenderText())
				{
					if (submenu_heighttextinput != "")
					{
						heightText(submenu_heighttextinput);
					}
				}

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
	
}

void Scene_Editor::update()
{
	m_entityManager.update();

	if (state == MENU)
	{

	}
	if (state == NEW)
	{
		if (submenu == nullptr)
		{
			createSubMenu();
		}

		if (m_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
		{
			SDL_Rect textbox{};
			textbox.x = submenu_heighttextbox->getComponent<TransformComponent>().position.x;
			textbox.y = submenu_heighttextbox->getComponent<TransformComponent>().position.y;
			textbox.w = submenu_heighttextbox->getComponent<SpriteComponent>().width;
			textbox.h = submenu_heighttextbox->getComponent<SpriteComponent>().height;

			SDL_Rect mousepoint{};
			mousepoint.x = m_inputManager->MousePos().x;
			mousepoint.y = m_inputManager->MousePos().y;
			mousepoint.w = 1;
			mousepoint.h = 1;

			if (SDL_HasIntersection(&textbox, &mousepoint))
			{
				if (!m_game->getTypeInput())
				{
					SDL_StartTextInput();
					m_game->setInputText(submenu_heighttextinput);
					m_game->ToggleTypeInput(true);

				}
			}
			else
			{
				if (m_game->getTypeInput())
				{
					SDL_StopTextInput();
					m_game->ToggleTypeInput(false);
				}
			}
		}

		if (m_game->getRenderText())
		{
			submenu_heighttextinput = m_game->getInputText();
		}
	}
}

void Scene_Editor::init()
{
	m_title = "Editor";
	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_RETURN, "SELECT");
	registerAction(SDL_SCANCODE_W, "UP");
	registerAction(SDL_SCANCODE_S, "DOWN");
	registerAction(SDL_SCANCODE_A, "LEFT");
	registerAction(SDL_SCANCODE_D, "RIGHT");

	m_entityManager = EntityManager();

	//camera = SDL_FRect{ 0, 0, static_cast<float>(m_graphics->SCREEN_WIDTH), static_cast<float>(m_graphics->SCREEN_HEIGHT) };
	createText("Editor", 100, Vec2(100, 75));
	createText("New Level", 50, Vec2(100, 300));
	createText("Load Level", 50, Vec2(100, 350));
	createCursor();
}

void Scene_Editor::onEnd()
{
	m_game->changeScene("Menu", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Editor::createText(std::string s, int size, Vec2 pos)
{
	auto text = m_entityManager.addEntity("Text");
	text->addComponent<TextComponent>(s, "Snes.ttf", size, 300, 200, 1, SDL_Color{ 255, 255, 255 });
	text->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
}

void Scene_Editor::createCursor()
{
	cursor = m_entityManager.addEntity("Cursor");
	cursor->addComponent<SpriteComponent>("cursor.png", 32, 32, 1);
	cursor->addComponent<TransformComponent>(Vec2(250, 300), Vec2(0, 0), 0);
}

void Scene_Editor::createSubMenu()
{
	submenu = m_entityManager.addEntity("SubMenu");
	submenu->addComponent<SpriteComponent>("menubox.png", 500, 250, 1);
	submenu->addComponent<TransformComponent>(Vec2((m_graphics->SCREEN_WIDTH / 2) - (500 / 2), (m_graphics->SCREEN_HEIGHT / 2) - (250 / 2)), Vec2(0, 0), 0);

	submenu_heighttext = m_entityManager.addEntity("SubMenu");
	submenu_heighttext->addComponent<TextComponent>("Height: (Min 16)", "eurostile.ttf", 32, 300, 300, 1, SDL_Color{0, 0, 0});
	auto& subparent = submenu_heighttext->addComponent<ParentComponent>(submenu);
	submenu_heighttext->addComponent<TransformComponent>(Vec2(subparent.parent->getComponent<TransformComponent>().position.x + 25, subparent.parent->getComponent<TransformComponent>().position.y + 25), Vec2(0, 0), 0);

	submenu_heighttextbox = m_entityManager.addEntity("SubMenu");
	submenu_heighttextbox->addComponent<SpriteComponent>("white.png", 250, 30, 1);
	submenu_heighttextbox->addComponent<ParentComponent>(submenu);
	submenu_heighttextbox->addComponent<TransformComponent>(Vec2(subparent.parent->getComponent<TransformComponent>().position.x + 25, subparent.parent->getComponent<TransformComponent>().position.y + 65), Vec2(0, 0), 0);

	heightText(submenu_heighttextinput);
}

void Scene_Editor::moveCursor(int i)
{
	cursor->getComponent<TransformComponent>().position.y = 300 + 50 * i;
}

void Scene_Editor::heightText(std::string s)
{
	if (submenu_heighttextinputentity)
	{
		submenu_heighttextinputentity->destroy();
	}

	submenu_heighttextinputentity = m_entityManager.addEntity("SubMenu");
	submenu_heighttextinputentity->addComponent<TextComponent>(s, "eurostile.ttf", 32, 300, 300, 1, SDL_Color{ 0, 0, 0 });
	submenu_heighttextinputentity->addComponent<ParentComponent>(submenu);
	submenu_heighttextinputentity->addComponent<TransformComponent>(Vec2(submenu_heighttext->getComponent<ParentComponent>().parent->getComponent<TransformComponent>().position.x + 25, submenu_heighttext->getComponent<ParentComponent>().parent->getComponent<TransformComponent>().position.y + 65), Vec2(0, 0), 0);
}

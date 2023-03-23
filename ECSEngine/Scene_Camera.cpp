#include "Scene_Camera.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"

Scene_Camera::Scene_Camera()
{
}

Scene_Camera::Scene_Camera(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Camera::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "QUIT")
		{
			onEnd();
		}
		if (action.getName() == "UP")
		{
			m_player->getComponent<TransformComponent>().velocity.y = -5;
		}
		if (action.getName() == "DOWN")
		{
			m_player->getComponent<TransformComponent>().velocity.y = 5;
		}
		if (action.getName() == "LEFT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = -5;
		}
		if (action.getName() == "RIGHT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = 5;
		}
	}

	if (action.getType() == "END")
	{
		if (action.getName() == "UP")
		{
			m_player->getComponent<TransformComponent>().velocity.y = 0;
		}
		if (action.getName() == "DOWN")
		{
			m_player->getComponent<TransformComponent>().velocity.y = 0;
		}
		if (action.getName() == "LEFT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = 0;
		}
		if (action.getName() == "RIGHT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = 0;
		}
	}
}

void Scene_Camera::sRender()
{
	for (auto& e : m_entityManager.getEntities())
	{
		auto& sprite = e->getComponent<SpriteComponent>();
		auto& transform = e->getComponent<TransformComponent>();

		sprite.destRect.x = static_cast<int>(transform.position.x - camera->x);
		sprite.destRect.y = static_cast<int>(transform.position.y - camera->y);
		sprite.destRect.w = sprite.width * sprite.scale;
		sprite.destRect.h = sprite.height * sprite.scale;
		m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
	}
}

void Scene_Camera::update()
{
	m_entityManager.update();

	camera->x = static_cast<int>(m_player->getComponent<TransformComponent>().position.x) - m_graphics->SCREEN_WIDTH / 2;
	camera->y = static_cast<int>(m_player->getComponent<TransformComponent>().position.y) - m_graphics->SCREEN_HEIGHT / 2;

	m_player->getComponent<TransformComponent>().position += m_player->getComponent<TransformComponent>().velocity;

	if (m_player->getComponent<TransformComponent>().position.x < 0)
	{
		m_player->getComponent<TransformComponent>().position.x = 0;
	}
	if (m_player->getComponent<TransformComponent>().position.x > LEVEL_WIDTH)
	{
		m_player->getComponent<TransformComponent>().position.x = LEVEL_WIDTH;
	}
	if (m_player->getComponent<TransformComponent>().position.y < 0)
	{
		m_player->getComponent<TransformComponent>().position.y = 0;
	}
	if (m_player->getComponent<TransformComponent>().position.y > LEVEL_HEIGHT)
	{
		m_player->getComponent<TransformComponent>().position.y = LEVEL_HEIGHT;
	}


	if (camera->x < 0)
		camera->x = 0;
	if (camera->y < 0)
		camera->y = 0;
	if (camera->x > LEVEL_WIDTH - m_graphics->SCREEN_WIDTH + 32)
		camera->x = LEVEL_WIDTH - m_graphics->SCREEN_WIDTH + 32;
	if (camera->y > LEVEL_HEIGHT - m_graphics->SCREEN_HEIGHT + 32)
		camera->y = LEVEL_HEIGHT - m_graphics->SCREEN_HEIGHT + 32;

	//std::cout << "Camera position: " << camera->x << " " << camera->y << std::endl;
	//std::cout << "Player position: " << m_player->getComponent<TransformComponent>().position.x << " " << m_player->getComponent<TransformComponent>().position.y << std::endl;
}

void Scene_Camera::init()
{
	m_title = "CAMERA";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_W, "UP");
	registerAction(SDL_SCANCODE_S, "DOWN");
	registerAction(SDL_SCANCODE_A, "LEFT");
	registerAction(SDL_SCANCODE_D, "RIGHT");
	m_entityManager = EntityManager();

	camera = new SDL_Rect{ 0,0,1024, 896 };
	createGridBackground();
	spawnPlayer();
}

void Scene_Camera::onEnd()
{
	delete camera;
	camera = nullptr;
	m_game->changeScene("Menu", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Camera::createGridBackground()
{
	for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 101; j++)
		{
			spawnTile(Vec2(j * 32,  i * 32));
		}
	}
}

void Scene_Camera::spawnTile(Vec2 vec)
{
	auto tile = m_entityManager.addEntity("Tile");
	tile->addComponent<SpriteComponent>("tile.png", 32, 32, 1);
	tile->addComponent<TransformComponent>(vec, Vec2(0, 0), 0);
}

void Scene_Camera::spawnPlayer()
{
	m_player = m_entityManager.addEntity("Player");
	m_player->addComponent<SpriteComponent>("dot.png", 32, 32, 1);
	m_player->addComponent<TransformComponent>(Vec2(m_graphics->SCREEN_WIDTH / 2, m_graphics->SCREEN_HEIGHT / 2), Vec2(0, 0), 0);
}

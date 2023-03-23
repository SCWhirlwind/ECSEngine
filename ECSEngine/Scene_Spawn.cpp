#include "Scene_Spawn.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"

Scene_Spawn::Scene_Spawn()
{
}

Scene_Spawn::Scene_Spawn(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Spawn::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "QUIT")
		{
			onEnd();
		}
		if (action.getName() == "INCREASE")
		{
			spawnInterval(0.05f);
		}
		if (action.getName() == "DECREASE")
		{
			spawnInterval(-0.05f);
		}
		else if (action.getName() == "UP")
		{
			m_spawner->getComponent<TransformComponent>().velocity.y = -5;
		}
		if (action.getName() == "DOWN")
		{
			m_spawner->getComponent<TransformComponent>().velocity.y = 5;
		}
	}

	if (action.getType() == "END")
	{
		if (action.getName() == "UP")
		{
			m_spawner->getComponent<TransformComponent>().velocity.y = 0;
		}
		if (action.getName() == "DOWN")
		{
			m_spawner->getComponent<TransformComponent>().velocity.y = 0;
		}
	}
}

void Scene_Spawn::sRender()
{
	for (auto& e : m_entityManager.getEntities())
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

void Scene_Spawn::update()
{
	m_entityManager.update();

	elapseTime += m_timer->DeltaTime();

	if (elapseTime >= spawn)
	{
		elapseTime = 0;
		createBullet();
	}

	m_spawner->getComponent<TransformComponent>().position += m_spawner->getComponent<TransformComponent>().velocity;

	for (auto& e : m_entityManager.getEntities("Bullet"))
	{
		e->getComponent<TransformComponent>().position += e->getComponent<TransformComponent>().velocity;

		if (e->getComponent<StatComponent>().timed >= 1.5f)
		{
			e->destroy();
		}
		else
		{
			e->getComponent<StatComponent>().timed += m_timer->DeltaTime();
		}
	}
}

void Scene_Spawn::init()
{
	m_title = "SPAWN";
	m_timer = Timer::Instance();

	std::srand(std::time(nullptr));
	yOffset = ((5 + 5) * ((float)std::rand() / RAND_MAX)) - 5;

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_W, "INCREASE");
	registerAction(SDL_SCANCODE_S, "DECREASE");
	registerAction(SDL_SCANCODE_Q, "UP");
	registerAction(SDL_SCANCODE_A, "DOWN");

	m_entityManager = EntityManager();
	createSpawner();
}

void Scene_Spawn::onEnd()
{
	m_timer = nullptr;
	m_game->changeScene("Menu", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Spawn::createSpawner()
{
	m_spawner = m_entityManager.addEntity("Spawner");
	m_spawner->addComponent<SpriteComponent>("spawner.png", 32, 32, 1);
	m_spawner->addComponent<TransformComponent>(Vec2(50, m_graphics->SCREEN_HEIGHT / 2), Vec2(0, 0), 0);
}

void Scene_Spawn::createBullet()
{
	yOffset = ((5 + 5) * ((float)std::rand() / RAND_MAX)) - 5;
	std::cout << yOffset << std::endl;
	auto e = m_entityManager.addEntity("Bullet");
	e->addComponent<SpriteComponent>("bullet.png", 32, 32, 1);
	e->addComponent<TransformComponent>(Vec2(m_spawner->getComponent<TransformComponent>().position), Vec2(5, yOffset), 0);
	e->addComponent<StatComponent>();

}

void Scene_Spawn::spawnInterval(float f)
{
	spawn += f;
}

void Scene_Spawn::moveSpawner(int v)
{
	m_spawner->getComponent<TransformComponent>().position.y += v;
}

#include "Scene_Ballbouncegame.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"

Scene_Ballbouncegame::Scene_Ballbouncegame()
{
}

Scene_Ballbouncegame::Scene_Ballbouncegame(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Ballbouncegame::sDoAction(const Action& action)
{
	const std::string actionName = action.getName();

	if (action.getType() == "START")
	{
		if (actionName == "QUIT")
		{
			onEnd();
		}
		if (actionName == "LEFT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = -5;
		}
		if (actionName == "RIGHT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = 5;
		}
	}
	if (action.getType() == "END")
	{
		if (actionName == "LEFT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = 0;
		}
		if (actionName == "RIGHT")
		{
			m_player->getComponent<TransformComponent>().velocity.x = 0;
		}
	}
}

void Scene_Ballbouncegame::sRender()
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

void Scene_Ballbouncegame::update()
{
	m_entityManager.update();

	m_player->getComponent<TransformComponent>().position += m_player->getComponent<TransformComponent>().velocity;

	if (m_player->getComponent<TransformComponent>().position.x > m_graphics->SCREEN_WIDTH - 32)
	{
		m_player->getComponent<TransformComponent>().position.x = m_graphics->SCREEN_WIDTH - 32;
	}
	if (m_player->getComponent<TransformComponent>().position.x < 0)
	{
		m_player->getComponent<TransformComponent>().position.x = 0;
	}
	
	sCollision();
	
	if (timer >= ballspawnrate)
	{
		spawnBall(1 + std::rand() % (5 - 1 + 1), (m_graphics->SCREEN_HEIGHT / 4), 1 + std::rand() % (7 - 1 + 1));
		timer = 0.0f;
	}
	else
	{
		timer += m_timer->DeltaTime();
	}
	

	for (auto& e : m_entityManager.getEntities("Ball"))
	{
		e->getComponent<TransformComponent>().position += e->getComponent<TransformComponent>().velocity;
		e->getComponent<TransformComponent>().velocity.y += gravity;

		if (e->getComponent<TransformComponent>().velocity.y > maxVel)
		{
			e->getComponent<TransformComponent>().velocity.y = maxVel;
		}

		if (e->getComponent<TransformComponent>().position.x >= m_graphics->SCREEN_WIDTH + e->getComponent<SpriteComponent>().scale * 32)
		{
			score += e->getComponent<SpriteComponent>().scale * 100;
			UpdateScore();
			e->destroy();
		}

		if (e->getComponent<TransformComponent>().position.y >= m_graphics->SCREEN_HEIGHT - e->getComponent<SpriteComponent>().scale * 32)
		{
			Vec2 invert = e->getComponent<TransformComponent>().velocity;

			e->getComponent<TransformComponent>().velocity.y = -invert.y;
		}
	}

	if (hit)
	{
		for (auto& e : m_entityManager.getEntities("Ball"))
		{
			e->destroy();
		}
		if (highscore < score)
		{
			highscore = score;
		}

		score = 0;
		HiScore();
		UpdateScore();
		hit = false;
	}
	
}

void Scene_Ballbouncegame::init()
{
	m_title = "BALLBOUNCEGAME";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_A, "LEFT");
	registerAction(SDL_SCANCODE_D, "RIGHT");

	m_entityManager = EntityManager();
	m_timer = Timer::Instance();

	std::srand(std::time(nullptr));

	spawnPlayer();
	CreateScore(100, Vec2(50, 50));
	CreateHiScore(100, Vec2(400, 50));
}

void Scene_Ballbouncegame::onEnd()
{
	m_timer = nullptr;
	m_hasEnded = true;
	m_game->changeScene("MENU", nullptr, true);

}

void Scene_Ballbouncegame::sCollision()
{
	Vec2 PlayerMidPos = mPhysics.midPos(m_player, m_player->getComponent<TransformComponent>().position);

	for (std::shared_ptr<Entity> e : m_entityManager.getEntities("Ball"))
	{
		Vec2 BallMidPos = mPhysics.midPos(e, e->getComponent<TransformComponent>().position);
		
		Vec2 dist = PlayerMidPos - BallMidPos;

		float distance = sqrt((dist.x * dist.x) + (dist.y * dist.y));

		float ballradius = static_cast<float>((e->getComponent<SpriteComponent>().height) * e->getComponent<SpriteComponent>().scale) / 2;
		float playerradius = static_cast<float>((m_player->getComponent<SpriteComponent>().height) * m_player->getComponent<SpriteComponent>().scale) / 2;
		
		if (distance <= ballradius + playerradius)
		{
			hit = true;
		}

	}
}

void Scene_Ballbouncegame::CreateScore(int size, Vec2 pos)
{
	scorelabel = m_entityManager.addEntity("Score");
	scorelabel->addComponent<TextComponent>(std::to_string(score), "Snes.ttf", size, 300, 200, 1, SDL_Color{ 255, 255, 255 });
	scorelabel->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
}

void Scene_Ballbouncegame::CreateHiScore(int size, Vec2 pos)
{
	hiscorelabel = m_entityManager.addEntity("Score");
	hiscorelabel->addComponent<TextComponent>(std::to_string(highscore), "Snes.ttf", size, 300, 200, 1, SDL_Color{ 255, 255, 255 });
	hiscorelabel->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
}

void Scene_Ballbouncegame::HiScore()
{
	Vec2 transform = hiscorelabel->getComponent<TransformComponent>().position;
	int scoresize = hiscorelabel->getComponent<TextComponent>().size;

	if (hiscorelabel)
	{
		hiscorelabel->destroy();
		hiscorelabel = m_entityManager.addEntity("Score");
		hiscorelabel->addComponent<TextComponent>(std::to_string(highscore), "Snes.ttf", scoresize, 300, 200, 1, SDL_Color{ 255, 255, 255 });
		hiscorelabel->addComponent<TransformComponent>(transform, Vec2(0, 0), 0);
	}
}

void Scene_Ballbouncegame::UpdateScore()
{
	Vec2 transform = scorelabel->getComponent<TransformComponent>().position;
	int scoresize = scorelabel->getComponent<TextComponent>().size;

	if (scorelabel)
	{
		scorelabel->destroy();
		scorelabel = m_entityManager.addEntity("Score");
		scorelabel->addComponent<TextComponent>(std::to_string(score), "Snes.ttf", scoresize, 300, 200, 1, SDL_Color{ 255, 255, 255 });
		scorelabel->addComponent<TransformComponent>(transform, Vec2(0, 0), 0);
	}
}

void Scene_Ballbouncegame::spawnBall(float speed, int height, int size)
{
	auto e = m_entityManager.addEntity("Ball");
	e->addComponent<SpriteComponent>("ball.png", 32, 32, size, false);
	e->addComponent<TransformComponent>(Vec2(-32 * size, height), Vec2(0,0), 0);
	e->getComponent<TransformComponent>().velocity.x = speed;
}

void Scene_Ballbouncegame::spawnPlayer()
{
	m_player = m_entityManager.addEntity("Player");
	m_player->addComponent<SpriteComponent>("dot.png", 32, 32, 1, false);
	m_player->addComponent<TransformComponent>(Vec2(m_graphics->SCREEN_WIDTH / 2, m_graphics->SCREEN_HEIGHT - 32), Vec2(0, 0), 0);
}

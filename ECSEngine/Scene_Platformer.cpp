#include "Scene_Platformer.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"

Scene_Platformer::Scene_Platformer()
{
}

Scene_Platformer::Scene_Platformer(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Platformer::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "QUIT")
		{
			onEnd();
		}
		if (action.getName() == "UP")
		{
			if (canJump)
			{
				canJump = !canJump;
				m_player->getComponent<TransformComponent>().velocity.y = -13;
			}
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
		if (action.getName() == "POS")
		{
			std::cout << "Transform: " << m_player->getComponent<TransformComponent>().position << std::endl;
		}
		if (action.getName() == "TEXTURE")
		{
			renderTexture = !renderTexture;
		}
		if (action.getName() == "COLLISION")
		{
			renderCollision = !renderCollision;
		}
		if (action.getName() == "POSITION")
		{
			std::cout << m_player->getComponent<TransformComponent>().position << std::endl;
		}
	}
	if (action.getType() == "END")
	{
		if (action.getName() == "UP")
		{
			
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

void Scene_Platformer::sRender()
{
	if (renderTexture)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			if (e->tag() != "Cell")
			{
				if (e->hasComponent<SpriteComponent>())
				{
					auto& sprite = e->getComponent<SpriteComponent>();
					auto& transform = e->getComponent<TransformComponent>();

					if (transform.position.x >= camera.x - 32 * SCALE && transform.position.x <= camera.x + m_graphics->SCREEN_WIDTH &&
						transform.position.y >= camera.y && transform.position.y <= camera.y + m_graphics->SCREEN_HEIGHT)
					{
						sprite.destRect.x = static_cast<int>(transform.position.x - camera.x);
						sprite.destRect.y = static_cast<int>(transform.position.y - camera.y);
						sprite.destRect.w = sprite.width * sprite.scale;
						sprite.destRect.h = sprite.height * sprite.scale;
						m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
					}
				}
			}
			else
			{
				if (e->tag() == "Cell")
				{
					auto& bucket = e->getComponent<CellComponent>().entityBucket;

					for (int i = 0; i < bucket.size(); ++i)
					{
						auto& sprite = bucket[i]->getComponent<SpriteComponent>();
						auto& transform = bucket[i]->getComponent<TransformComponent>();

						if (transform.position.x >= camera.x - 32 * SCALE && transform.position.x <= camera.x + m_graphics->SCREEN_WIDTH &&
							transform.position.y >= camera.y && transform.position.y <= camera.y + m_graphics->SCREEN_HEIGHT)
						{
							sprite.destRect.x = static_cast<int>(transform.position.x - camera.x);
							sprite.destRect.y = static_cast<int>(transform.position.y - camera.y);
							sprite.destRect.w = sprite.width * sprite.scale;
							sprite.destRect.h = sprite.height * sprite.scale;
							m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
						}
					}
				}
			}
		}

		for (auto& e : m_entityManager.getEntities("Player"))
		{
			auto& sprite = e->getComponent<SpriteComponent>();
			auto& transform = e->getComponent<TransformComponent>();

			sprite.destRect.x = static_cast<int>(transform.position.x - camera.x);
			sprite.destRect.y = static_cast<int>(transform.position.y - camera.y);
			sprite.destRect.w = sprite.width * sprite.scale;
			sprite.destRect.h = sprite.height * sprite.scale;
			m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
		}
	}

	

	if (renderCollision)
	{
		for (auto& e : m_entityManager.getEntities("Cell"))
		{
			if (e->hasComponent<CellComponent>())
			{
				auto& cell = e->getComponent<CellComponent>();
				auto& transform = e->getComponent<TransformComponent>();

				if (transform.position.x >= camera.x - 32 && transform.position.x <= camera.x + m_graphics->SCREEN_WIDTH &&
					transform.position.y >= camera.y && transform.position.y <= camera.y + m_graphics->SCREEN_HEIGHT)
				{
					//cell.destRect.x = static_cast<int>(transform.position.x - camera.x);
					//cell.destRect.y = static_cast<int>(transform.position.y - camera.y);
					//cell.destRect.w = cell.width * cell.scale;
					//cell.destRect.h = cell.height * cell.scale;
					//m_graphics->DrawTexture(cell.texture, &cell.srcRect, &cell.destRect, transform.angle);
				}
			}
		}

		for (auto& e : m_entityManager.getEntities())
		{
			if (e->hasComponent<ColliderComponent>())
			{
				auto& box = e->getComponent<ColliderComponent>().size;
				auto& transform = e->getComponent<TransformComponent>().position;
				SDL_Rect rect = {transform.x - camera.x, transform.y - camera.y, box.x, box.y};
				m_graphics->DrawRect(rect);
			}
		}
	}
	/*for (auto& e : m_entityManager.getEntities("Wall"))
	{
		if (e->hasComponent<ColliderComponent>())
		{
			auto& box = e->getComponent<ColliderComponent>().size;
			auto& transform = e->getComponent<TransformComponent>().position;
			Vec2 point1 = e->getComponent<TransformComponent>().position;

			Vec2 point2 = e->getComponent<TransformComponent>().position;
			point2 += Vec2(32, 0);

			Vec2 point[4];

			point[0] = transform;

			point[1] = transform;
			point[1].x += box.x;

			point[2] = transform;
			point[2].y += box.y;

			point[3] = transform + box;
			
			for (int i = 0; i < 4; i++)
			{
				int t = i + 1;
				if (t < 3)
				{
					rayCast(lineIntersect(m_player->getComponent<TransformComponent>().position, m_inputManager->MousePos(), point[i], point[t]));
				}
				else
				{
					rayCast(lineIntersect(m_player->getComponent<TransformComponent>().position, m_inputManager->MousePos(), point[i], point[0]));
				}
			}
			
		}
	}
	*/
}

void Scene_Platformer::update()
{
	m_entityManager.update();

	sPlayerUpdate();
	sCamera();
	sCollision();

	Vec2 MousePos = m_inputManager->MousePos();
	//std::cout << "Mouse pos:" << std::floor((MousePos.x + camera.x) / 32 / SCALE) << " " << std::floor((MousePos.y + camera.y) / 32 / SCALE) << std::endl;

	if (m_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
	{
		Vec2 PosToArray = Vec2(std::floor((MousePos.x + camera.x) / 32 / SCALE), std::floor((MousePos.y + camera.y) / 32 / SCALE));

		//std::vector<std::shared_ptr<Entity>> entityBucket = getBucket(PosToArray);
		//std::cout << MousePos << std::endl;
		//std::cout << "POSTOARRAY: " << PosToArray << std::endl;
		//createWall(PosToArray);
		//std::cout << entityBucket.size() << std::endl;
	}
	m_player->getComponent<TransformComponent>().prevPosition = m_player->getComponent<TransformComponent>().position;
	

}

void Scene_Platformer::init()
{
	m_title = "PLATFORMER";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_W, "UP");
	registerAction(SDL_SCANCODE_S, "DOWN");
	registerAction(SDL_SCANCODE_A, "LEFT");
	registerAction(SDL_SCANCODE_D, "RIGHT");
	registerAction(SDL_SCANCODE_Q, "POS");
	registerAction(SDL_SCANCODE_T, "TEXTURE");
	registerAction(SDL_SCANCODE_C, "COLLISION");
	registerAction(SDL_SCANCODE_F, "POSITION");

	m_entityManager = EntityManager();

	camera = SDL_FRect{ 0, 0, static_cast<float>(m_graphics->SCREEN_WIDTH), static_cast<float>(m_graphics->SCREEN_HEIGHT) };

	createLevel();
	m_player = createEntity(Vec2(m_graphics->SCREEN_WIDTH / 2, m_graphics->SCREEN_HEIGHT / 2), "Player");
}

void Scene_Platformer::onEnd()
{
	m_game->changeScene("Menu", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Platformer::sPlayerUpdate()
{

	auto& transform = m_player->getComponent<TransformComponent>();
	auto& playerState = m_player->getComponent<StateComponent>();

	if (playerState.state == "onAir")
	{
		transform.velocity.y += gravityVelocity;
	}


	if (transform.velocity.y > 10)
	{
		transform.velocity.y = 10;
	}
	else if (transform.velocity.y < -13)
	{
		transform.velocity.y = -13;
	}

	m_player->getComponent<TransformComponent>().position += m_player->getComponent<TransformComponent>().velocity;



	if (m_player->getComponent<TransformComponent>().position.x < 0)
	{
		m_player->getComponent<TransformComponent>().position.x = 0;
	}
	if (m_player->getComponent<TransformComponent>().position.x > LEVEL_WIDTH - 32)
	{
		m_player->getComponent<TransformComponent>().position.x = LEVEL_WIDTH - 32;
	}
	if (m_player->getComponent<TransformComponent>().position.y < 0)
	{
		m_player->getComponent<TransformComponent>().position.y = 0;
	}
	if (m_player->getComponent<TransformComponent>().position.y > LEVEL_HEIGHT - 32)
	{
		m_player->getComponent<TransformComponent>().position.y = LEVEL_HEIGHT - 32;
	}
}

void Scene_Platformer::sCamera()
{
	camera.x = m_player->getComponent<TransformComponent>().position.x - m_graphics->SCREEN_WIDTH / 2.0f;
	camera.y = m_player->getComponent<TransformComponent>().position.y - m_graphics->SCREEN_HEIGHT / 2.0f;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > LEVEL_WIDTH - m_graphics->SCREEN_WIDTH)
		camera.x = LEVEL_WIDTH - m_graphics->SCREEN_WIDTH;
	if (camera.y > LEVEL_HEIGHT - m_graphics->SCREEN_HEIGHT)
		camera.y = LEVEL_HEIGHT - m_graphics->SCREEN_HEIGHT;
}

void Scene_Platformer::sCollision()
{

	for (auto& e : m_entityManager.getEntities("Cell"))
	{
		Vec2 col = m_physics.getOverlap(m_player, e);

		if (col.x > 0 && col.y > 0)
		{
			Vec2 prevCol = m_physics.getPreviousOverlap(m_player, e);


			//corner collision happened
			if (prevCol.x < 0 && prevCol.y < 0)
			{
				//coming from above
				//player.y has to be less than e.y
				if (m_player->getComponent<TransformComponent>().position.y < e->getComponent<TransformComponent>().position.y)
				{
					m_player->getComponent<TransformComponent>().position.y -= col.y;
					m_player->getComponent<TransformComponent>().velocity.y = 0;
				}
				//coming from below
				//player.y has to be greater than e.y
				//if (m_player->getComponent<TransformComponent>().position.y > e->getComponent<TransformComponent>().position.y)
				//{
				//	m_player->getComponent<TransformComponent>().position.y += col.y;
				//	m_player->getComponent<TransformComponent>().velocity.y = 0;
				//}
			}
			
			//vertical collision happened
			else if (prevCol.x > 0)
			{
				auto& transform = e->getComponent<TransformComponent>();
				//coming from above
				//player.y has to be less than e.y
				if (m_player->getComponent<TransformComponent>().position.y < e->getComponent<TransformComponent>().position.y)
				{
					m_player->getComponent<TransformComponent>().position.y -= col.y;
					m_player->getComponent<TransformComponent>().velocity.y = 0;
					canJump = true;
				}
				//coming from below
				//player.y has to be greater than e.y
				if (m_player->getComponent<TransformComponent>().position.y > e->getComponent<TransformComponent>().position.y)
				{
					m_player->getComponent<TransformComponent>().position.y += col.y;
					m_player->getComponent<TransformComponent>().velocity.y = 0;
				}
			}
			
			//horizontal collision happened
			else if (prevCol.y > 0)
			{
				//coming from the left side
				//player.x has to be less than e.x
				if (m_player->getComponent<TransformComponent>().position.x < e->getComponent<TransformComponent>().position.x)
				{
					m_player->getComponent<TransformComponent>().position.x -= col.x;
				}
			
				//coming from the right side
				//player.x has to be greater then e.x
				if (m_player->getComponent<TransformComponent>().position.x > e->getComponent<TransformComponent>().position.x)
				{
					m_player->getComponent<TransformComponent>().position.x += col.x;
				}
			}
		}
	}
}

void Scene_Platformer::createLevel()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			auto tile = createEntity(Vec2(j * SCALED_TILE_WIDTH, i * SCALED_TILE_HEIGHT), "Wall");

			addToBucket(Vec2(i, j), tile);
		}
	}
}

void Scene_Platformer::updateBuckets()
{

}

void Scene_Platformer::addToBucket(Vec2 pos, std::shared_ptr<Entity> entity)
{
	cell_map.insert({getPosKey(pos), entity});
}

std::string Scene_Platformer::getPosKey(Vec2 pos)
{
	return  std::to_string(static_cast<int>(pos.x)) + "," + std::to_string(static_cast<int>(pos.y));
}

std::shared_ptr<Entity> Scene_Platformer::createEntity(Vec2 pos, std::string name)
{
	auto entity = m_entityManager.addEntity(name);

	if (name == "Wall")
	{
		entity->addComponent<TransformComponent>(pos, pos, 0);
		entity->addComponent<SpriteComponent>("tile3.png", TILE_WIDTH, TILE_HEIGHT, SCALE);
		entity->addComponent<ColliderComponent>(Vec2(SCALED_TILE_WIDTH, SCALED_TILE_HEIGHT));
	}
	else if (name == "Player")
	{
		entity->addComponent<SpriteComponent>("dot.png", 32, 32, 1);
		entity->addComponent<TransformComponent>(pos, pos, 0);
		Vec2 size = Vec2(entity->getComponent<SpriteComponent>().width, entity->getComponent<SpriteComponent>().height);
		entity->addComponent<ColliderComponent>(size);
		entity->addComponent<StateComponent>();
	}

	return entity;
}

void Scene_Platformer::rayCast(Vec2 a)
{
	auto& transform = m_player->getComponent<TransformComponent>();

	Vec2 playerPos = transform.position;

	m_graphics->DrawLine(playerPos.x - camera.x + 16, playerPos.y - camera.y + 16, a.x, a.y);
}

Vec2 Scene_Platformer::lineIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d)
{
	Vec2 r = b - a;
	Vec2 s = d - c;

	float rxs = r.x * s.y - r.y * s.x;

	Vec2 cma = c - a;

	float cmaxs = cma.x * s.y - cma.y * s.x;
	float t = cmaxs / rxs;

	float cmaxr = cma.x * r.y - cma.y * r.x;

	float u = cmaxr / rxs;

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return Vec2(a.x + t * r.x, a.y + t * r.y);
	}

	return Vec2(0, 0);
}
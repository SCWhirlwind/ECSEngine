#include "Scene_Poker.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"
#include <iostream>
#include <sstream>
#include <fstream>

Scene_Poker::Scene_Poker()
{
}

Scene_Poker::Scene_Poker(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Poker::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "QUIT")
		{
			onEnd();
		}
	}

	if (action.getType() == "END")
	{
	}
}

void Scene_Poker::sRender()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<SpriteComponent>() && e->hasComponent<TransformComponent>())
		{
			if (e->getComponent<SpriteComponent>().isActive)
			{
				renderEntities(e);
			}
		}
		else if (e->hasComponent<TextComponent>())
		{
			if (e->getComponent<TextComponent>().isActive)
			{
				renderEntities(e);
			}
		}
	}
}

void Scene_Poker::update()
{
	m_entityManager.update();

	switch (m_state)
	{
	case Scene_Poker::State::TEST:
		break;

	case Scene_Poker::State::BUILD:

		if (isBuying)
		{
			if (selectTiles())
			{
				if (m_inputManager->MouseButtonReleased(InputManager::MOUSE_BUTTON::left))
				{
					createTower();

					for (int i = 0; i < 4; i++)
					{
						auto cursorEntity = m_entityManager.getEntity(selectorCursor[i]);

						if (cursorEntity)
						{
							cursorEntity->getComponent<SpriteComponent>().isActive = false;
						}
					}

					isBuying = false;
					m_state = Scene_Poker::State::DEAL;
				}
			}
		}

		break;

	case Scene_Poker::State::DEAL:

		if (dealDelay >= dealTimer)
		{
			dealTimer += m_timer->DeltaTime();
			return;
		}


		if (!dealt)
		{
			auto dealBackgroundEntity = m_entityManager.getEntity(dealBackground);

			if (dealBackgroundEntity)
			{
				dealBackgroundEntity->getComponent<SpriteComponent>().isActive = true;
			}

			for (auto& e : refreshButton)
			{
				auto buttonEntity = m_entityManager.getEntity(e);
				if (buttonEntity)
				{
					buttonEntity->getComponent<SpriteComponent>().isActive = true;
					buttonEntity->getComponent<ButtonComponent>().isActive = true;
				}
			}

			auto acceptButtonEntity = m_entityManager.getEntity(acceptButton);

			if (acceptButtonEntity)
			{
				acceptButtonEntity->getComponent<SpriteComponent>().isActive = true;
				acceptButtonEntity->getComponent<ButtonComponent>().isActive = true;
			}

			dealHand();
			dealt = true;
		}

		if (handresult)
		{
			currentTime -= m_timer->DeltaTime();
			int elapsedTime = static_cast<int>(currentTime);

			if (previousTime != elapsedTime)
			{
				createTimerText(std::to_string(elapsedTime), 50, Vec2(100, 50));
				previousTime = elapsedTime;
			}

			if (currentTime < 0)
			{
				handresult = false;

				auto dealBackgroundEntity = m_entityManager.getEntity(dealBackground);

				if (dealBackgroundEntity)
				{
					dealBackgroundEntity->getComponent<SpriteComponent>().isActive = false;
				}

				for (EntityID e : refreshButton)
				{
					auto buttonEntity = m_entityManager.getEntity(e);
					if (buttonEntity)
					{
						buttonEntity->getComponent<SpriteComponent>().isActive = false;
						buttonEntity->getComponent<ButtonComponent>().isActive = false;
					}
				}

				auto acceptButtonEntity = m_entityManager.getEntity(acceptButton);

				if (acceptButtonEntity)
				{
					acceptButtonEntity->getComponent<SpriteComponent>().isActive = false;
					acceptButtonEntity->getComponent<ButtonComponent>().isActive = false;
				}

				auto timerTextEntity = m_entityManager.getEntity(timerText);

				if (timerTextEntity)
				{
					timerTextEntity->getComponent<TextComponent>().isActive = false;
				}

				auto handTextEntity = m_entityManager.getEntity(handText);
				if (handTextEntity)
				{
					handTextEntity->getComponent<TextComponent>().isActive = false;
				}

				for (EntityID e : m_playerHand)
				{
					auto entity = m_entityManager.getEntity(e);
					if (entity)
					{
						entity->getComponent<CardComponent>().erank->destroy();
						entity->getComponent<CardComponent>().esuit->destroy();
						entity->destroy();
					}
				}

				m_state = Scene_Poker::State::WAVE;
				enemiesPerWave = waves[level].count;
				enemyHP = waves[level].hp;
				enemySpeed = waves[level].speed;
				index = waves[level].index;
			}
		}
		break;

	case Scene_Poker::State::WAVE:

		if (!spawningDone)
		{
			if (enemiesSpawned < enemiesPerWave)
			{
				if (timeGap <= timer)
				{
					auto spawnPointEntity = m_entityManager.getEntity(spawnPoint);

					if (spawnPointEntity)
					{
						createEnemy(spawnPointEntity->getComponent<PointComponent>().point, enemyHP, enemySpeed, index);
						aliveEnemies++;
					}

					timer = 0.0f;
					enemiesSpawned++;
					
				}
				else
				{
					timer += m_timer->DeltaTime();
				}
			}
			else
			{
				spawningDone = true;
				level++;
			}
		}
		

		for (EntityID it : enemyList)
		{
			auto enemyEntity = m_entityManager.getEntity(it);

			if (enemyEntity)
			{
				auto& transform = enemyEntity->getComponent<TransformComponent>().position;
				auto& enemyComp = enemyEntity->getComponent<EnemyComponent>();
				Vec2 midPos = sPoker.midPos(enemyEntity, transform);
				auto waypointEntity = m_entityManager.getEntity(waypoints[enemyComp.waypointIndex]);
				
				if (waypointEntity)
				{
					const Vec2& currentWaypoint = waypointEntity->getComponent<PointComponent>().point;

					Vec2 toTarget = currentWaypoint - midPos;

					if (toTarget.MagnitudeSqr() > 0.1f)
					{
						Vec2 direction = toTarget.Normalize();
						transform += direction * enemyComp.speed;

						updateEnemiesInRange(enemyEntity);
					}
					else
					{
						if (enemyComp.waypointIndex++ == 12)
						{
							enemyList.erase(std::remove(enemyList.begin(), enemyList.end(), enemyEntity->id()), enemyList.end());
							enemyEntity->destroy();
							//subtract lives

							aliveEnemies--;
						}
					}
				}

				if (enemyComp.hp <= 0)
				{
					enemyList.erase(std::remove(enemyList.begin(), enemyList.end(), enemyEntity->id()), enemyList.end());
					enemyEntity->destroy();
					aliveEnemies--;
				}
			}
		}
		
		for (EntityID tower : towerList)
		{
			auto towerEntity = m_entityManager.getEntity(tower);
			
			if (towerEntity)
			{
				towerAttack(towerEntity);
			}
		}

		for (EntityID projectile : projectiles)
		{
			auto projectileEntity = m_entityManager.getEntity(projectile);
			
			if (projectileEntity)
			{
				auto& transform = projectileEntity->getComponent<TransformComponent>().position;
				auto& projectileComb = projectileEntity->getComponent<ProjectileComponent>();
				auto& targetTransform = projectileComb.target->getComponent<TransformComponent>().position;

				Vec2 midPos = sPoker.midPos(projectileEntity, transform);
				Vec2 targetMidPos = sPoker.midPos(projectileComb.target, targetTransform);

				float dx = targetMidPos.x - midPos.x;
				float dy = targetMidPos.y - midPos.y;

				float distsqr = dx * dx + dy * dy;
				float hitRadiusSq = 10.0f;

				if (distsqr <= hitRadiusSq)
				{
					projectileComb.target->getComponent<EnemyComponent>().hp -= projectileComb.damage;
					projectileEntity->destroy();
				}
				else
				{
					float dist = std::sqrt(distsqr);
					float moveX = (dx / dist) * projectileComb.speed;
					float moveY = (dy / dist) * projectileComb.speed;
					transform.x += moveX;
					transform.y += moveY;
				}

				if (!projectileComb.target->isActive())
				{
					projectileEntity->destroy();
				}
			}
		}

		if (spawningDone && aliveEnemies == 0)
		{
			m_state = Scene_Poker::State::BUILD;
			resetLoop();
		}

		break;
	default:
		break;
	}

	for (auto& e : m_entityManager.getEntities())
	{
		if (e->tag() == "Button")
		{
			if (e->hasComponent<ButtonComponent>())
			{
				if (e->getComponent<ButtonComponent>().isActive)
				{
					sUICollision(e);
				}
			}
		}
	}
}

void Scene_Poker::renderEntities(std::shared_ptr<Entity> entity)
{
	auto& transform = entity->getComponent<TransformComponent>();

	if (entity->hasComponent<SpriteComponent>())
	{
		auto& sprite = entity->getComponent<SpriteComponent>();

		sprite.destRect.x = static_cast<int>(transform.position.x);
		sprite.destRect.y = static_cast<int>(transform.position.y);
		sprite.destRect.w = sprite.width * sprite.scale;
		sprite.destRect.h = sprite.height * sprite.scale;
		m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
	}
	else if (entity->hasComponent<TextComponent>())
	{
		auto& sprite = entity->getComponent<TextComponent>();

		sprite.destRect.x = static_cast<int>(transform.position.x);
		sprite.destRect.y = static_cast<int>(transform.position.y);
		sprite.destRect.w = sprite.width * sprite.scale;
		sprite.destRect.h = sprite.height * sprite.scale;
		m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
	}
}

void Scene_Poker::init()
{
	m_title = "POKER";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");

	m_entityManager = EntityManager();

	m_timer = Timer::Instance();

	std::srand(std::time(nullptr));

	createUI();
	createWaypoint();

	loadWaveData();

	m_state = Scene_Poker::State::BUILD;
}

void Scene_Poker::onEnd()
{
	m_hasEnded = true;
	m_game->changeScene("MENU", nullptr, true);
}

void Scene_Poker::sUICollision(std::shared_ptr<Entity> object)
{
	if (object->tag() == "Button" && object->hasComponent<ButtonComponent>())
	{
		if (m_inputManager->MouseButtonReleased(InputManager::MOUSE_BUTTON::left))
		{
			Vec2 mousePos = m_inputManager->MousePos();
			Vec2 buttonPos = object->getComponent<TransformComponent>().position;
			if (mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + object->getComponent<SpriteComponent>().width &&
				mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + object->getComponent<SpriteComponent>().height)
			{
				object->getComponent<ButtonComponent>().isClicked = true;
			}
		}
	}

	auto buyTowerButtonEntity = m_entityManager.getEntity(buyTowerButton);

	if(buyTowerButtonEntity)
	{
		if (buyTowerButtonEntity->getComponent<ButtonComponent>().isClicked)
		{
			isBuying = true;
			for (int i = 0; i < 4; i++)
			{
				auto selectorEntity = m_entityManager.getEntity(selectorCursor[i]);
				if (selectorEntity)
				{
					selectorEntity->getComponent<SpriteComponent>().isActive = true;
				}
			}

			buyTowerButtonEntity->getComponent<ButtonComponent>().isClicked = false;
			buyTowerButtonEntity->getComponent<ButtonComponent>().isActive = false;
		}
	}
	

	int index = 0;

	for (EntityID e : refreshButton)
	{
		auto refreshButtonEntity = m_entityManager.getEntity(e);
		if (refreshButtonEntity)
		{
			if (refreshButtonEntity->getComponent<ButtonComponent>().isClicked)
			{
				auto m_playerHandEntity = m_entityManager.getEntity(m_playerHand[index]);
				changeCard(m_playerHandEntity, index);
				refreshButtonEntity->getComponent<ButtonComponent>().isClicked = false;
				refreshButtonEntity->getComponent<ButtonComponent>().isActive = false;
				refreshButtonEntity->getComponent<SpriteComponent>().srcRect.x = 150;
			}
			index++;
		}
	}

	auto acceptButtonEntity = m_entityManager.getEntity(acceptButton);
	if (acceptButtonEntity)
	{
		if (acceptButtonEntity->getComponent<ButtonComponent>().isClicked)
		{
			acceptButtonEntity->getComponent<ButtonComponent>().isClicked = false;
			acceptButtonEntity->getComponent<ButtonComponent>().isActive = false;

			for (EntityID e : refreshButton)
			{
				auto refreshButtonEntity = m_entityManager.getEntity(e);
				if (refreshButtonEntity)
				{
					refreshButtonEntity->getComponent<ButtonComponent>().isClicked = false;
					refreshButtonEntity->getComponent<ButtonComponent>().isActive = false;
					refreshButtonEntity->getComponent<SpriteComponent>().srcRect.x = 150;
				}
				
			}

			handResult();
		}
	}
}

void Scene_Poker::addTile(int x, int y, std::shared_ptr<Entity> tile)
{
	if (x <= 50 && y <= 50)
	{
		if (grid.size() <= y) grid.resize(y + 1);
		if (grid[y].size() <= x) grid[y].resize(x + 1);

		
		grid[y][x] = GridCell(tile->id());

		if (tile->getComponent<SpriteComponent>().textureName == "dirt.png")
		{
			grid[y][x].buildable = false;
		}
		else
		{
			grid[y][x].buildable = true;
		}
	}
}

bool Scene_Poker::selectTiles()
{
	int mouseX = m_inputManager->MousePos().x;
	int mouseY = m_inputManager->MousePos().y;

	mouseX = mouseX % 24;
	mouseY = mouseY % 24;

	Vec2 mouseGridPos = m_inputManager->MousePos();
	mouseGridPos.x = mouseGridPos.x / 24;
	mouseGridPos.y = mouseGridPos.y / 24;
	mouseGridPos.x = static_cast<int>(mouseGridPos.x);
	mouseGridPos.y = static_cast<int>(mouseGridPos.y);

	bool cursors[4] = { false, false, false, false };

	if (mouseGridPos.x >= 0 && mouseGridPos.x <= 43 && mouseGridPos.y >= 0 && mouseGridPos.y <= 39)
	{
		auto selectorCursorEntity = m_entityManager.getEntity(selectorCursor[0]);
		auto selectorCursorEntity1 = m_entityManager.getEntity(selectorCursor[1]);
		auto selectorCursorEntity2 = m_entityManager.getEntity(selectorCursor[2]);
		auto selectorCursorEntity3 = m_entityManager.getEntity(selectorCursor[3]);

		if (selectorCursorEntity && selectorCursorEntity1 && selectorCursorEntity2 && selectorCursorEntity3)
		{
			// Original Mouse Position
			selectorCursorEntity->getComponent<TransformComponent>().position = mouseGridPos * 24;

			if (mouseX < 12)
			{
				if (mouseGridPos.x == 0)
				{
					selectorCursorEntity1->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 0);
				}
				else
				{
					selectorCursorEntity1->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(24, 0);
				}
			}
			if (mouseY < 12)
			{
				if (mouseGridPos.y == 0)
				{
					selectorCursorEntity2->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(0, 24);
				}
				else
				{
					selectorCursorEntity2->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(0, 24);
				}
			}
			if (mouseX > 12)
			{
				if (mouseGridPos.x == 43)
				{
					selectorCursorEntity1->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(24, 0);
				}
				else
				{
					selectorCursorEntity1->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 0);
				}
			}
			if (mouseY > 12)
			{
				if (mouseGridPos.y == 39)
				{
					selectorCursorEntity2->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(0, 24);
				}
				else
				{
					selectorCursorEntity2->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(0, 24);
				}
			}

			if (mouseX < 12 && mouseY < 12)
			{
				if (mouseGridPos.x == 0 && mouseGridPos.y != 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
				}
				else if (mouseGridPos.x == 0 && mouseGridPos.y == 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
				}
				else if (mouseGridPos.x != 0 && mouseGridPos.y == 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
				}
				else
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(24, 24);
				}
			}
			if (mouseX > 12 && mouseY < 12)
			{
				if (mouseGridPos.x == 0 && mouseGridPos.y == 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
				}
				else if (mouseGridPos.x != 0 && mouseGridPos.x != 43 && mouseGridPos.y == 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
				}
				else if (mouseGridPos.x == 43 && mouseGridPos.y != 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
				}
				else if (mouseGridPos.x == 43 && mouseGridPos.y == 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
				}
				else
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
				}
			}
			if (mouseX < 12 && mouseY > 12)
			{
				if (mouseGridPos.x == 0 && mouseGridPos.y != 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
				}
				else if (mouseGridPos.x == 0 && mouseGridPos.y == 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
				}
				else if (mouseGridPos.x != 0 && mouseGridPos.x != 43 && mouseGridPos.y == 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
				}
				else if (mouseGridPos.x != 0 && mouseGridPos.x == 43 && mouseGridPos.y == 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
				}
				else
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
				}
			}
			if (mouseX > 12 && mouseY > 12)
			{
				if (mouseGridPos.x == 43 && mouseGridPos.y == 0)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
				}
				else if (mouseGridPos.x == 0 && mouseGridPos.y == 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
				}
				else if (mouseGridPos.x == 43 && mouseGridPos.y != 0 && mouseGridPos.y != 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
				}
				else if (mouseGridPos.x == 43 && mouseGridPos.y == 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
				}
				else if (mouseGridPos.x != 43 && mouseGridPos.x != 0 && mouseGridPos.y == 39)
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
				}
				else
				{
					selectorCursorEntity3->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
				}
			}
		}
		
		int index = 0;

		for (EntityID e : selectorCursor)
		{
			auto entity = m_entityManager.getEntity(e);
			if (entity)
			{
				if (!grid[entity->getComponent<TransformComponent>().position.y / 24][entity->getComponent<TransformComponent>().position.x / 24].buildable)
				{
					entity->getComponent<SpriteComponent>().srcRect.x = 32;
					cursors[index] = false;
				}
				else
				{
					entity->getComponent<SpriteComponent>().srcRect.x = 0;
					cursors[index] = true;
					cursorlocation[index] = entity->getComponent<TransformComponent>().position;
				}
				index++;
			}
		}
	}

	for (bool val : cursors)
	{
		if (!val)
		{
			return false;
		}
	}	

	return true;
}

void Scene_Poker::createUI()
{
	createBackground();

	auto button = m_entityManager.addEntity("Button");
	button->addComponent<TransformComponent>(Vec2(1068, 856), Vec2(0, 0), 0);
	button->addComponent<SpriteComponent>("buybutton.png", 200, 100, 1, false);
	button->addComponent<ButtonComponent>();
	button->getComponent<ButtonComponent>().isActive = true;

	buyTowerButton = button->id();

	createCursor();
	createDealBackground();
	createRefreshButton();
	createAcceptButton();
}

void Scene_Poker::createBackground()
{
	std::string  x;
	std::string  y;
	std::string  name;
	std::ifstream myfile("map.txt");

	if (myfile.is_open())
	{
		std::string line;

		while (std::getline(myfile, line))
		{
			size_t start = line.find('(');
			size_t comma = line.find(',', start);
			size_t end = line.find(')', comma);

			if (start != std::string::npos && comma != std::string::npos && end != std::string::npos)
			{
				std::stringstream ss(line.substr(start + 1, comma - start - 1));
				ss >> x;

				ss.clear();
				ss.str(line.substr(comma + 1, end - comma - 1));
				ss >> y;

				name = line.substr(end + 2); // end + 2 to skip ") "

				auto tile = m_entityManager.addEntity("Tile");
				tile->addComponent<TransformComponent>(Vec2(std::stoi(x), std::stoi(y)), Vec2(std::stoi(x), std::stoi(y)), 0);
				tile->addComponent<SpriteComponent>(name, 32, 32, 0.75, false);
				//std::cout << "X: " << std::stoi(x) / 24 << " Y: " << std::stoi(y) / 24 << std::endl;
				addTile(std::stoi(x) / 24, std::stoi(y) / 24, tile);
			}
		}
		myfile.close();
	}
}

void Scene_Poker::createDealBackground()
{
	auto dback = m_entityManager.addEntity("DealBackground");
	dback->addComponent<TransformComponent>(Vec2(0, 0), Vec2(0, 0), 0);
	dback->addComponent<SpriteComponent>("dealbackground.png", 1280, 700, 1, false);
	dback->getComponent<SpriteComponent>().isActive = false;
	dealBackground = dback->id();
}

void Scene_Poker::createCursor()
{
	for (int i = 0; i < 4; i++)
	{
		auto cursor = m_entityManager.addEntity("SelectorCursor");
		cursor->addComponent<TransformComponent>(Vec2(0, 0), Vec2(0, 0), 0);
		cursor->addComponent<SpriteComponent>("selector.png", 32, 32, 0.75, false);
		cursor->getComponent<SpriteComponent>().isActive = false;
		selectorCursor[i] = cursor->id();
	}
}

void Scene_Poker::createRefreshButton()
{
	for (int i = 0; i < 5; i++)
	{
		auto button = m_entityManager.addEntity("Button");
		button->addComponent<SpriteComponent>("refresh.png", 150, 64, 1, false);
		button->addComponent<TransformComponent>(Vec2(115 + (i * 225), 575), Vec2(0, 0), 0);
		button->addComponent<ButtonComponent>();
		button->getComponent<ButtonComponent>().isActive = false;
		button->getComponent<SpriteComponent>().isActive = false;
		refreshButton[i] = button->id();
	}
}

void Scene_Poker::createAcceptButton()
{
	auto button = m_entityManager.addEntity("Button");
	button->addComponent<SpriteComponent>("accept.png", 100, 100, 1, false);
	button->addComponent<TransformComponent>(Vec2(1150, 30), Vec2(0, 0), 0);
	button->addComponent<ButtonComponent>();
	button->getComponent<ButtonComponent>().isActive = false;
	button->getComponent<SpriteComponent>().isActive = false;
	acceptButton = button->id();
}

void Scene_Poker::loadWaveData()
{
	std::ifstream myfile("Wave.txt");
	if (myfile.is_open())
	{
		std::string line;

		while (std::getline(myfile, line))
		{
			WaveData w;
			char comma;

			std::istringstream iss(line);
			if (iss >> w.count >> comma >> w.hp >> comma >> w.speed >> comma >> w.index)
				waves.push_back(w);
		}
	}
}

void Scene_Poker::createTower()
{
	auto tower = m_entityManager.addEntity("Tower");

	Vec2 LowestPosition = cursorlocation[0];

	for (int i = 0; i < 4; i++)
	{
		if (cursorlocation[i].x <= LowestPosition.x && cursorlocation[i].y <= LowestPosition.y)
		{
			LowestPosition = cursorlocation[i];
		}
		grid[cursorlocation[i].y / 24][cursorlocation[i].x / 24].buildable = false;
		grid[cursorlocation[i].y / 24][cursorlocation[i].x / 24].cell = tower->id();
	}

	tower->addComponent<TransformComponent>(LowestPosition, Vec2(0, 0), 0);
	tower->addComponent<SpriteComponent>("tower.png", 48, 48, 1, false);
	tower->getComponent<SpriteComponent>().isActive = true;

	towerList.push_back(tower->id());
}

void Scene_Poker::dealHand()
{
	for (int i = 0; i < 5; i++)
	{
		auto card = m_entityManager.addEntity("Card");

		m_playerHand[i] = card->id();

		int randrank = 0 + std::rand() % (7 - 0 + 1);
		CardComponent::Rank rank = static_cast<CardComponent::Rank>(randrank);
		int randsuit = 0 + std::rand() % (2 - 0 + 1);
		CardComponent::Suit suit = static_cast<CardComponent::Suit>(randsuit);
		card->addComponent<CardComponent>(rank, suit);
		card->addComponent<SpriteComponent>("card.png", 100, 175, 2, false);
		card->addComponent<TransformComponent>(Vec2(90+(i*225), 200), Vec2(0, 0), 0);
		createface(card);
	}
}

void Scene_Poker::clearHand()
{
	for (EntityID e : m_playerHand)
	{
		auto entity = m_entityManager.getEntity(e);
		if (entity)
		{
			entity->destroy();
		}
	}

	for (auto& e : m_sortHand)
	{
		if (e->isActive())
		{
			e->destroy();
		}
	}
}

void Scene_Poker::handResult()
{
	int index = 0;

	for (EntityID e : m_playerHand)
	{
		auto entity = m_entityManager.getEntity(e);
		if (entity)
		{
			m_sortHand[index] = entity;
			index++;
		}
	}

	sPoker.sortHand(m_sortHand);
	std::pair result = sPoker.checkHand(m_sortHand);

	int resultt = sPoker.checkHighCard(m_sortHand, result.second);
	std::string resultString;
	int highCard = 0;
	switch (resultt)
	{
		case 0:
			resultString = "Seven High";
			highCard = 0;
			break;
		case 1:
			resultString = "Eight High";
			highCard = 1;
			break;
		case 2:
			resultString = "Nine High";
			highCard = 2;
			break;
		case 3:
			resultString = "Ten High";
			highCard = 3;
			break;
		case 4:
			resultString = "Jack High";
			highCard = 4;
			break;
		case 5:
			resultString = "Queen High";
			highCard = 5;
			break;
		case 6:
			resultString = "King High";
			highCard = 6;
			break;
		case 7:
			resultString = "Ace High";
			highCard = 8;
			break;
	}

	createHandText("Your hand is a " + result.first + " and " + resultString, 50, Vec2(100, 100));

	auto towerEntity = m_entityManager.getEntity(towerList.back());
	if (towerEntity)
	{
		setTowerStats(result.second, highCard, towerEntity);
	}
	handresult = true;
}

void Scene_Poker::changeCard(std::shared_ptr<Entity> card, int selection)
{
	int randrank = 0 + std::rand() % (7 - 0 + 1);
	CardComponent::Rank newrank = static_cast<CardComponent::Rank>(randrank);
	int randsuit = 0 + std::rand() % (2 - 0 + 1);
	CardComponent::Suit newsuit = static_cast<CardComponent::Suit>(randsuit);
	card->getComponent<CardComponent>().rank = newrank;
	card->getComponent<CardComponent>().suit = newsuit;
	createface(card);
}

void Scene_Poker::createface(std::shared_ptr<Entity> card)
{
	auto& cardface = card->getComponent<CardComponent>();

	if (cardface.erank && cardface.esuit)
	{
		cardface.erank->destroy();
		cardface.esuit->destroy();
	}

	cardface.erank = m_entityManager.addEntity("Rank");
	cardface.esuit = m_entityManager.addEntity("Suit");


	cardface.erank->addComponent<TransformComponent>();
	cardface.erank->getComponent<TransformComponent>().position = card->getComponent<TransformComponent>().position;
	cardface.esuit->addComponent<TransformComponent>();
	cardface.esuit->getComponent<TransformComponent>().position = card->getComponent<TransformComponent>().position;

	switch (cardface.rank)
	{
	case 0:
		cardface.erank->addComponent<SpriteComponent>("seven.png", 100, 175, 2, false);
		break;
	case 1:
		cardface.erank->addComponent<SpriteComponent>("eight.png", 100, 175, 2, false);
		break;
	case 2:
		cardface.erank->addComponent<SpriteComponent>("nine.png", 100, 175, 2, false);
		break;
	case 3:
		cardface.erank->addComponent<SpriteComponent>("ten.png", 100, 175, 2, false);
		break;
	case 4:
		cardface.erank->addComponent<SpriteComponent>("jack.png", 100, 175, 2, false);
		break;
	case 5:
		cardface.erank->addComponent<SpriteComponent>("queen.png", 100, 175, 2, false);
		break;
	case 6:
		cardface.erank->addComponent<SpriteComponent>("king.png", 100, 175, 2, false);
		break;
	case 7:
		cardface.erank->addComponent<SpriteComponent>("ace.png", 100, 175, 2, false);
		break;
	}

	switch (cardface.suit)
	{
	case 0:
		cardface.esuit->addComponent<SpriteComponent>("red.png", 100, 175, 2, false);
		break;
	case 1:
		cardface.esuit->addComponent<SpriteComponent>("black.png", 100, 175, 2, false);
		break;
	case 2:
		cardface.esuit->addComponent<SpriteComponent>("white2.png", 100, 175, 2, false);
		break;
	}
}

void Scene_Poker::createHandText(std::string s, int size, Vec2 pos)
{
	auto handTextEntity = m_entityManager.getEntity(handText);

	if (handTextEntity)
	{
		handTextEntity->destroy();
	}
	handTextEntity = m_entityManager.addEntity("Text");
	handTextEntity->addComponent<TextComponent>(s, "Snes.ttf", size, 700, 300, 1, SDL_Color{ 255, 255, 255 });
	handTextEntity->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
	handText = handTextEntity->id();
}

void Scene_Poker::createTimerText(std::string s, int size, Vec2 pos)
{
	auto timerTextEntity = m_entityManager.getEntity(timerText);

	if (timerTextEntity)
	{
		timerTextEntity->destroy();
	}
	timerTextEntity = m_entityManager.addEntity("Text");
	timerTextEntity->addComponent<TextComponent>(s, "Snes.ttf", size, 700, 300, 1, SDL_Color{ 255, 255, 255 });
	timerTextEntity->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
	timerText = timerTextEntity->id();
}

void Scene_Poker::setTowerStats(int hand, int high, std::shared_ptr<Entity> tower)
{
	tower->addComponent<TowerComponent>(hand, high, 128, 0.4f);
}

EntityID Scene_Poker::createPoints(Vec2 p)
{
	auto point = m_entityManager.addEntity("point");
	point->addComponent<PointComponent>(p);
	return point->id();
	return 0;
}

void Scene_Poker::createWaypoint()
{
	spawnPoint = createPoints(Vec2(370, -50));
	waypoints.push_back(createPoints(Vec2(384, 840)));
	waypoints.push_back(createPoints(Vec2(98, 840)));
	waypoints.push_back(createPoints(Vec2(98, 550)));
	waypoints.push_back(createPoints(Vec2(960, 550)));
	waypoints.push_back(createPoints(Vec2(960, 840)));
	waypoints.push_back(createPoints(Vec2(676, 840)));
	waypoints.push_back(createPoints(Vec2(676, 120)));
	waypoints.push_back(createPoints(Vec2(960, 120)));
	waypoints.push_back(createPoints(Vec2(960, 406)));
	waypoints.push_back(createPoints(Vec2(98, 406)));
	waypoints.push_back(createPoints(Vec2(98, 120)));
	waypoints.push_back(createPoints(Vec2(530, 120)));
	waypoints.push_back(createPoints(Vec2(530, 1000)));
}

void Scene_Poker::createEnemy(Vec2 p, int hp, int speed, int index)
{
	auto enemy = m_entityManager.addEntity("Enemy");
	enemy->addComponent<TransformComponent>(p, Vec2(0, 0), 0);
	enemy->addComponent<SpriteComponent>("enemy.png", 32, 32, 1, false);
	enemy->getComponent<SpriteComponent>().isActive = true;
	enemy->addComponent<EnemyComponent>(hp, speed, index);
	enemyList.push_back(enemy->id());
}

void Scene_Poker::createProjectile(Vec2 p, int damage, std::shared_ptr<Entity> target, int speed)
{
	auto projectile = m_entityManager.addEntity("Projectile");
	projectile->addComponent<TransformComponent>(p, Vec2(0, 0), 0);
	projectile->addComponent<SpriteComponent>("projectile.png", 16, 16, 1, false);
	projectile->getComponent<SpriteComponent>().isActive = true;
	projectile->addComponent<ProjectileComponent>(damage, target, speed);
	projectiles.push_back(projectile->id());	
}

void Scene_Poker::updateEnemiesInRange(std::shared_ptr<Entity> enemy)
{
	for (auto t : towerList)
	{
		auto tower = m_entityManager.getEntity(t);
		if (tower)
		{
			float dx = enemy->getComponent<TransformComponent>().position.x - tower->getComponent<TransformComponent>().position.x;
			float dy = enemy->getComponent<TransformComponent>().position.y - tower->getComponent<TransformComponent>().position.y;
			float distSq = dx * dx + dy * dy;
			float rangeSq = tower->getComponent<TowerComponent>().range * tower->getComponent<TowerComponent>().range;

			bool inRange = false;

			if (distSq <= rangeSq)
			{
				inRange = true;
			}
			
			if (inRange)
			{
				if (std::find(tower->getComponent<TowerComponent>().enemiesIDInRange.begin(), tower->getComponent<TowerComponent>().enemiesIDInRange.end(), enemy->id()) != tower->getComponent<TowerComponent>().enemiesIDInRange.end())
				{

				}
				else
				{
					tower->getComponent<TowerComponent>().enemiesIDInRange.push_back(enemy->id());
				}
			}
			else
			{
				tower->getComponent<TowerComponent>().enemiesIDInRange.erase(std::remove(tower->getComponent<TowerComponent>().enemiesIDInRange.begin(), tower->getComponent<TowerComponent>().enemiesIDInRange.end(), enemy->id()), tower->getComponent<TowerComponent>().enemiesIDInRange.end());
			}
		}
	}
}

void Scene_Poker::towerAttack(std::shared_ptr<Entity> tower)
{
	auto& towerComp = tower->getComponent<TowerComponent>();

	if (towerComp.attackSpeed <= towerComp.attackTimer)
	{
		if (!towerComp.enemiesIDInRange.empty())
		{
			auto enemy = m_entityManager.getEntity(towerComp.enemiesIDInRange.front());

			if (enemy)
			{
				if (!enemy->isActive())
				{
					towerComp.enemiesIDInRange.erase(towerComp.enemiesIDInRange.begin());
				}

				Vec2 midPos = tower->getComponent<TransformComponent>().position - Vec2(16, 16);
				createProjectile(sPoker.midPos(tower, midPos), towerComp.damage, enemy, 5);
				towerComp.attackTimer = 0.0f;
			}
			else
			{
				towerComp.enemiesIDInRange.erase(towerComp.enemiesIDInRange.begin());
			}
		}
	}
	else
	{
		towerComp.attackTimer += m_timer->DeltaTime();
	}
}

void Scene_Poker::resetLoop()
{
	auto buyTowerButtonEntity = m_entityManager.getEntity(buyTowerButton);

	if (buyTowerButtonEntity)
	{
		buyTowerButtonEntity->getComponent<ButtonComponent>().isActive = true;
	}

	for (EntityID e : refreshButton)
	{
		auto buttonEntity = m_entityManager.getEntity(e);
		if (buttonEntity)
		{
			buttonEntity->getComponent<ButtonComponent>().isActive = false;
			buttonEntity->getComponent<SpriteComponent>().srcRect.x = 0;
		}
	}

	auto acceptButtonEntity = m_entityManager.getEntity(acceptButton);

	if (acceptButtonEntity)
	{
		acceptButtonEntity->getComponent<ButtonComponent>().isActive = false;
		acceptButtonEntity->getComponent<SpriteComponent>().srcRect.x = 0;
	}

	dealt = false;
	clearHand();
	handresult = false;
	dealTimer = 0;
	previousTime = 0;
	currentTime = 5.0f;
	spawningDone = false;
	enemiesSpawned = 0;
}

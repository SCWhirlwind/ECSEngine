#include "Scene_GOL.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"

Scene_GOL::Scene_GOL()
{
}

Scene_GOL::Scene_GOL(GameManager* game) : Scene(game)
{
	init();
}

void Scene_GOL::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "QUIT")
		{
			onEnd();
		}

		if (action.getName() == "RANDOM")
		{
			randomize();
		}

		if (action.getName() == "TOGGLE")
		{
			toggle = !toggle;
		}

		if (action.getName() == "INCREASE")
		{
			speed += 0.10f;
		}

		if (action.getName() == "DECREASE")
		{
			speed -= 0.10f;
		}
	}

	if (action.getType() == "END")
	{
	}
}

void Scene_GOL::sRender()
{
	for (auto& e : m_entityManager.getEntities())
	{
		auto& sprite = e->getComponent<SpriteComponent>();
		auto& transform = e->getComponent<TransformComponent>();

		if (sprite.hasAnim)
		{
			if (e->getComponent<GolComponent>().framecounter < 4 && e->getComponent<GolComponent>().currentState == true)
			{
				sprite.srcRect.x = sprite.srcRect.w * e->getComponent<GolComponent>().framecounter;
				e->getComponent<GolComponent>().framecounter++;
			}
			else if (e->getComponent<GolComponent>().framecounter >= 0 && e->getComponent<GolComponent>().currentState == false)
			{
				sprite.srcRect.x = sprite.srcRect.w * e->getComponent<GolComponent>().framecounter;
				e->getComponent<GolComponent>().framecounter--;
			}
		}

		sprite.destRect.x = static_cast<int>(transform.position.x);
		sprite.destRect.y = static_cast<int>(transform.position.y);
		sprite.destRect.w = sprite.width * sprite.scale;
		sprite.destRect.h = sprite.height * sprite.scale;
		m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
	}
}

void Scene_GOL::update()
{
	m_entityManager.update();

	Vec2 MousePos = m_inputManager->MousePos();

	if (m_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
	{
		Vec2 PosToArray = Vec2(std::floor((MousePos.x) / 8), std::floor((MousePos.y) / 8));

		if (PosToArray.x >= 0 && PosToArray.x <= 160 && PosToArray.y >= 0 && PosToArray.y <= 128)
		{
			int x = static_cast<int>(PosToArray.x);
			int y = static_cast<int>(PosToArray.y);
			auto& tile = golArray[y][x];

			if (tile->getComponent<GolComponent>().currentState == false)
			{
				tile->getComponent<GolComponent>().currentState = true;
			}
			else if (tile->getComponent<GolComponent>().currentState = true)
			{
				tile->getComponent<GolComponent>().currentState = false;
			}
		}
	}

	if (toggle)
	{
		if (timer > speed)
		{
			setCurrentToPrev();
			life();
			timer = 0;
		}
		else
		{
			timer += m_timer->DeltaTime();
		}
	}
}

void Scene_GOL::init()
{
	m_title = "GOL";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_Q, "RANDOM");
	registerAction(SDL_SCANCODE_R, "TOGGLE");
	registerAction(SDL_SCANCODE_S, "DECREASE");
	registerAction(SDL_SCANCODE_W, "INCREASE");

	m_entityManager = EntityManager();
	m_timer = Timer::Instance();

	std::srand(std::time(nullptr));

	createGrid();
}

void Scene_GOL::onEnd()
{
	m_timer = nullptr;
	m_hasEnded = true;
	m_game->changeScene("MENU", nullptr, true);
}

void Scene_GOL::createGrid()
{
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 160; j++)
		{
			golArray[i][j] = createTile(j, i);
		}
	}
}

std::shared_ptr<Entity> Scene_GOL::createTile(int col, int row)
{
	auto tile = m_entityManager.addEntity("Tile");
	tile->addComponent<SpriteComponent>("animtile.png", 8, 8, 1, true);
	tile->addComponent<TransformComponent>(Vec2(col * 8, row * 8), Vec2(0, 0), 0);
	tile->addComponent<GolComponent>();
	tile->getComponent<SpriteComponent>().AddAnimation("Anim", 0, 4, 100);
	tile->getComponent<SpriteComponent>().Play("Anim");
	
	return tile;
}

void Scene_GOL::setCurrentToPrev()
{
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 160; j++)
		{
			golArray[i][j]->getComponent<GolComponent>().previousState = golArray[i][j]->getComponent<GolComponent>().currentState;
		}
	}
}

int Scene_GOL::checkNeighbors(int row, int column)
{
	int cellAlive = 0;

	if (row == 0 && column == 0) //Top Left Corner
	{
		if (golArray[row][column + 1]->getComponent<GolComponent>().previousState == true) //Right
		{
			cellAlive++;
		}

		if (golArray[row + 1][column + 1]->getComponent<GolComponent>().previousState == true) //Bottom Right
		{
			cellAlive++;
		}

		if (golArray[row + 1][column]->getComponent<GolComponent>().previousState == true) //Bottom
		{
			cellAlive++;
		}
	}

	else if (row == 0 && column != 0 && column != 160 - 1) //Top Side
	{
		if (golArray[row][column - 1]->getComponent<GolComponent>().previousState == true) //Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column - 1]->getComponent<GolComponent>().previousState == true) //Bottom Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column]->getComponent<GolComponent>().previousState == true) //Bottom
		{
			cellAlive++;
		}

		if (golArray[row + 1][column + 1]->getComponent<GolComponent>().previousState == true) //Bottom Right
		{
			cellAlive++;
		}

		if (golArray[row][column + 1]->getComponent<GolComponent>().previousState == true) //Right
		{
			cellAlive++;
		}
	}

	else if (row == 0 && column == 160 - 1) //Top Right Corner
	{
		if (golArray[row][column - 1]->getComponent<GolComponent>().previousState == true) //Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column - 1]->getComponent<GolComponent>().previousState == true) //Bottom Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column]->getComponent<GolComponent>().previousState == true) //Bottom
		{
			cellAlive++;
		}
	}

	else if (column == 160 - 1 && row != 0 && row != 128 - 1) //Right Side
	{
		if (golArray[row - 1][column]->getComponent<GolComponent>().previousState == true) //Top
		{
			cellAlive++;
		}

		if (golArray[row - 1][column - 1]->getComponent<GolComponent>().previousState == true) //Top left
		{
			cellAlive++;
		}

		if (golArray[row][column - 1]->getComponent<GolComponent>().previousState == true) //Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column - 1]->getComponent<GolComponent>().previousState == true) //Bottom Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column]->getComponent<GolComponent>().previousState == true) //Bottom
		{
			cellAlive++;
		}
	}

	else if (row == 128 - 1 && column == 160 - 1) //Bottom Right Corner
	{
		if (golArray[row - 1][column]->getComponent<GolComponent>().previousState == true) //Top
		{
			cellAlive++;
		}

		if (golArray[row - 1][column - 1]->getComponent<GolComponent>().previousState == true) //Top left
		{
			cellAlive++;
		}

		if (golArray[row][column - 1]->getComponent<GolComponent>().previousState == true) //Left
		{
			cellAlive++;
		}
	}

	else if (row == 128 - 1 && column != 0 && column != 160) //Bottom Side
	{
		if (golArray[row][column + 1]->getComponent<GolComponent>().previousState == true) //Right
		{
			cellAlive++;
		}

		if (golArray[row - 1][column + 1]->getComponent<GolComponent>().previousState == true) //Top Right
		{
			cellAlive++;
		}

		if (golArray[row - 1][column]->getComponent<GolComponent>().previousState == true) //Top
		{
			cellAlive++;
		}

		if (golArray[row - 1][column - 1]->getComponent<GolComponent>().previousState == true) //Top left
		{
			cellAlive++;
		}

		if (golArray[row][column - 1]->getComponent<GolComponent>().previousState == true) //Left
		{
			cellAlive++;
		}
	}

	else if (row == 128 - 1 && column == 0) //Bottom Left Corner
	{
		if (golArray[row - 1][column]->getComponent<GolComponent>().previousState == true) //Top
		{
			cellAlive++;
		}

		if (golArray[row - 1][column + 1]->getComponent<GolComponent>().previousState == true) //Top Right
		{
			cellAlive++;
		}

		if (golArray[row][column + 1]->getComponent<GolComponent>().previousState == true) //Right
		{
			cellAlive++;
		}
	}

	else if (column == 0 && row != 0 && row != 128) //Left Side
	{
		if (golArray[row - 1][column]->getComponent<GolComponent>().previousState == true) //Top
		{
			cellAlive++;
		}

		if (golArray[row - 1][column + 1]->getComponent<GolComponent>().previousState == true) //Top Right
		{
			cellAlive++;
		}

		if (golArray[row][column + 1]->getComponent<GolComponent>().previousState == true) //Right
		{
			cellAlive++;
		}

		if (golArray[row + 1][column + 1]->getComponent<GolComponent>().previousState == true) //Bottom Right
		{
			cellAlive++;
		}

		if (golArray[row + 1][column]->getComponent<GolComponent>().previousState == true) //Bottom
		{
			cellAlive++;
		}
	}

	else
	{
		if (golArray[row - 1][column]->getComponent<GolComponent>().previousState == true) //Top
		{
			cellAlive++;
		}

		if (golArray[row - 1][column + 1]->getComponent<GolComponent>().previousState == true) //Top Right
		{
			cellAlive++;
		}

		if (golArray[row][column + 1]->getComponent<GolComponent>().previousState == true) //Right
		{
			cellAlive++;
		}

		if (golArray[row + 1][column + 1]->getComponent<GolComponent>().previousState == true) //Bottom Right
		{
			cellAlive++;
		}

		if (golArray[row + 1][column]->getComponent<GolComponent>().previousState == true) //Bottom
		{
			cellAlive++;
		}

		if (golArray[row - 1][column - 1]->getComponent<GolComponent>().previousState == true) //Top left
		{
			cellAlive++;
		}

		if (golArray[row][column - 1]->getComponent<GolComponent>().previousState == true) //Left
		{
			cellAlive++;
		}

		if (golArray[row + 1][column - 1]->getComponent<GolComponent>().previousState == true) //Bottom Left
		{
			cellAlive++;
		}
	}
	return cellAlive;
}

void Scene_GOL::life()
{
	for (int i = 0; i < 128; ++i)
	{
		for (int j = 0; j < 160; ++j)
		{
			int total = checkNeighbors(i, j);

			if (total < 2 || total > 3)
			{
				golArray[i][j]->getComponent<GolComponent>().currentState = false;
			}
			else if (total == 2)
			{
				golArray[i][j]->getComponent<GolComponent>().currentState = golArray[i][j]->getComponent<GolComponent>().previousState;
			}
			else if (total == 3)
			{
				golArray[i][j]->getComponent<GolComponent>().currentState = true;
			}
		}
	}
}

void Scene_GOL::randomize()
{
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 160; j++)
		{
			int random = 1 + std::rand() % (5 - 1 + 1);

			if (random > 4)
			{
				golArray[i][j]->getComponent<GolComponent>().currentState = true;
			}
		}
	}
}

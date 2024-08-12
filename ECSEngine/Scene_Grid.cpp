#include "Scene_Grid.h"
#include "Scene_Menu.h"
#include "GameManager.h"
#include "ComponentHeaders.h"
#include <iostream>

Scene_Grid::Scene_Grid()
{
}

Scene_Grid::Scene_Grid(GameManager* game) : Scene(game)
{
	init();
}

void Scene_Grid::sDoAction(const Action& action)
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

void Scene_Grid::sRender()
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

void Scene_Grid::update()
{
	m_entityManager.update();
	//std::cout << gridCoord().x << " " << gridCoord().y << std::endl;
	tileHover();
	checkMousePress();
}

void Scene_Grid::init()
{
	m_title = "GRID";

	m_tileArray.resize(cols, std::vector<std::shared_ptr<Entity>>(rows));

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");

	m_entityManager = EntityManager();
	createIsoGrid();
}

void Scene_Grid::onEnd()
{
	m_hasEnded = true;
	m_game->changeScene("MENU", nullptr, true);
}

std::shared_ptr<Entity> Scene_Grid::spawnTile(Vec2 vec)
{
	auto tile = m_entityManager.addEntity("Tile");
	tile->addComponent<SpriteComponent>("tile2.png", 32, 32, 1, false);
	tile->addComponent<TransformComponent>(vec, Vec2(0, 0), 0);
	tile->addComponent<StatComponent>();

	return tile;
	//std::cout << tile->id() << ": " << tile->getComponent<TransformComponent>().position.x << " " << tile->getComponent<TransformComponent>().position.y << std::endl;
}

void Scene_Grid::createGrid()
{
	int height = m_graphics->SCREEN_HEIGHT;
	int width = m_graphics->SCREEN_WIDTH;

	Vec2 screenMidpoint = Vec2(static_cast<int>(width / 2), static_cast<int>(height / 2));

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			spawnTile(Vec2((screenMidpoint.x + (i * 32) - (rows * 32) / 2), (screenMidpoint.y + (j * 32)) - (cols * 32) / 2));
		}
	}
}

void Scene_Grid::createIsoGrid()
{
	int height = m_graphics->SCREEN_HEIGHT;
	int width = m_graphics->SCREEN_WIDTH;

	int tilew = 32;
	int tileh = 32;

	Vec2 screenMidpoint = Vec2(static_cast<float>(width / 2), static_cast<float>(height / 2));

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Vec2 pos = Vec2(screenMidpoint.x + (j * 0.5 * tilew + i * -0.5 * tilew) - 32 / 2, screenMidpoint.y + (j * 0.25 * tileh + i * 0.25 * tileh) - (cols * 32 / 2) / 2);
			m_tileArray[i][j] = spawnTile(pos);
		}
	}
}

void Scene_Grid::checkMousePress()
{
	if (m_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
	{
		std::cout << m_inputManager->MousePos().x << " " << m_inputManager->MousePos().y << std::endl;
		//tileHover();
	}
}

void Scene_Grid::tileHover()
{
	auto tile = grabTile();

	if (tile != nullptr)
	{

		auto& hoverTile = tile->getComponent<StatComponent>();
		if (hoverTile.hovered == false)
		{
			tile->getComponent<TransformComponent>().position.y -= 5;
			hoverTile.hovered = true;

			if (m_PrevHover != nullptr)
			{
				m_PrevHover->getComponent<StatComponent>().hovered = false;
				m_PrevHover->getComponent<TransformComponent>().position.y += 5;
				m_PrevHover = tile;
			}
			else
			{
				m_PrevHover = tile;
			}
		}
	}
}

std::shared_ptr<Entity> Scene_Grid::grabTile()
{
	Vec2 mousePos = gridCoord();

	if (mousePos.x >= 0 && mousePos.x < rows && mousePos.y >= 0 && mousePos.y < cols)
	{
		return m_tileArray[mousePos.y][mousePos.x];
	}
	else
	{
		if (m_PrevHover != nullptr)
		{
			if (m_PrevHover->getComponent<StatComponent>().hovered)
			{
				m_PrevHover->getComponent<StatComponent>().hovered = false;
				m_PrevHover->getComponent<TransformComponent>().position.y += 5;
				m_PrevHover = nullptr;
			}
		}
	}

	return nullptr;
}

Vec2 Scene_Grid::gridCoord()
{
	float a = 0.5 * 32;
	float b = -0.5 * 32;
	float c = 0.25 * 32;
	float d = 0.25 * 32;

	float det = (1 / (a * d - b * c));

	float inva = det * d;
	float invb = det * -b;
	float invc = det * -c;
	float invd = det * a;

	float posx = m_inputManager->MousePos().x * inva + m_inputManager->MousePos().y * invb - (rows * 0.75) - 4; //+ m_graphics->SCREEN_WIDTH / 2 - 32 / 2;
	float posy = m_inputManager->MousePos().x * invc + m_inputManager->MousePos().y * invd + 4;

	return Vec2(static_cast<int>(posx), static_cast<int>(posy));
}

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
		case Scene_Poker::State::BUILD:

			if (isBuying)
			{
				if (selectTiles()) 
				{
					if (m_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
					{
						createTower();

						for (int i = 0; i < 4; i++)
						{
							selectorCursor[i]->getComponent<SpriteComponent>().isActive = false;
						}

						isBuying = false;
						m_state = Scene_Poker::State::DEAL;
					}
				}
			}

			break;

		case Scene_Poker::State::DEAL:

			if (!dealt)
			{
				dealBackground->getComponent<SpriteComponent>().isActive = true;

				for (auto& e : refreshButton)
				{
					e->getComponent<SpriteComponent>().isActive = true;
					e->getComponent<ButtonComponent>().isActive = true;
				}

				acceptButton->getComponent<SpriteComponent>().isActive = true;
				acceptButton->getComponent<ButtonComponent>().isActive = true;

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
					dealBackground->getComponent<SpriteComponent>().isActive = false;

					for (auto& e : refreshButton)
					{
						e->getComponent<SpriteComponent>().isActive = false;
						e->getComponent<ButtonComponent>().isActive = false;
					}

					acceptButton->getComponent<SpriteComponent>().isActive = false;
					acceptButton->getComponent<ButtonComponent>().isActive = false;

					timerText->getComponent<TextComponent>().isActive = false;
					handText->getComponent<TextComponent>().isActive = false;

					for (auto& e : m_playerHand)
					{
						e->getComponent<CardComponent>().erank->destroy();
						e->getComponent<CardComponent>().esuit->destroy();
						e->destroy();
					}

					m_state = Scene_Poker::State::WAVE;
				}
			}

			break;
		case Scene_Poker::State::WAVE:
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

	if (buyTowerButton->getComponent<ButtonComponent>().isClicked)
	{
		isBuying = true;
		for (int i = 0; i < 4; i++)
		{
			selectorCursor[i]->getComponent<SpriteComponent>().isActive = true;
		}
		
		buyTowerButton->getComponent<ButtonComponent>().isClicked = false;
		buyTowerButton->getComponent<ButtonComponent>().isActive = false;
	}

	int index = 0;

	for (auto& e : refreshButton)
	{
		if (e->getComponent<ButtonComponent>().isClicked)
		{
			changeCard(m_playerHand, index);
			e->getComponent<ButtonComponent>().isClicked = false;
			e->getComponent<ButtonComponent>().isActive = false;
			e->getComponent<SpriteComponent>().srcRect.x = 150;
		}
		index++;
	}

	if (acceptButton->getComponent<ButtonComponent>().isClicked)
	{
		acceptButton->getComponent<ButtonComponent>().isClicked = false;
		acceptButton->getComponent<ButtonComponent>().isActive = false;

		for (auto& e : refreshButton)
		{
			e->getComponent<ButtonComponent>().isClicked = false;
			e->getComponent<ButtonComponent>().isActive = false;
			e->getComponent<SpriteComponent>().srcRect.x = 150;
		}

		handResult();
	}
}

void Scene_Poker::addTile(int x, int y, std::shared_ptr<Entity> tile)
{
	if (x <= 50 && y <= 50)
	{
		if (grid.size() <= y) grid.resize(y + 1);
		if (grid[y].size() <= x) grid[y].resize(x + 1);

		
		grid[y][x] = GridCell(tile);

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

		// Original Mouse Position
		selectorCursor[0]->getComponent<TransformComponent>().position = mouseGridPos * 24;

		if (mouseX < 12)
		{
			if (mouseGridPos.x == 0)
			{
				selectorCursor[1]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 0);
			}
			else
			{
				selectorCursor[1]->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(24, 0);
			}
		}
		if (mouseY < 12)
		{
			if (mouseGridPos.y == 0)
			{
				selectorCursor[2]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(0, 24);
			}
			else
			{
				selectorCursor[2]->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(0, 24);
			}
		}
		if (mouseX > 12)
		{
			if (mouseGridPos.x == 43)
			{
				selectorCursor[1]->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(24, 0);
			}
			else
			{
				selectorCursor[1]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 0);
			}
		}
		if (mouseY > 12)
		{
			if (mouseGridPos.y == 39)
			{
				selectorCursor[2]->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(0, 24);
			}
			else
			{
				selectorCursor[2]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(0, 24);
			}
		}

		if (mouseX < 12 && mouseY < 12)
		{
			if (mouseGridPos.x == 0 && mouseGridPos.y != 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
			}
			else if (mouseGridPos.x == 0 && mouseGridPos.y == 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
			}
			else if (mouseGridPos.x != 0 && mouseGridPos.y == 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
			}
			else
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) - Vec2(24, 24);
			}
		}
		if (mouseX > 12 && mouseY < 12)
		{
			if (mouseGridPos.x == 0 && mouseGridPos.y == 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
			}
			else if (mouseGridPos.x != 0 && mouseGridPos.x != 43 && mouseGridPos.y == 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
			}
			else if (mouseGridPos.x == 43 && mouseGridPos.y != 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
			}
			else if (mouseGridPos.x == 43 && mouseGridPos.y == 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
			}
			else
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
			}
		}
		if (mouseX < 12 && mouseY > 12)
		{
			if (mouseGridPos.x == 0 && mouseGridPos.y != 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
			}
			else if (mouseGridPos.x == 0 && mouseGridPos.y == 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
			}
			else if (mouseGridPos.x != 0 && mouseGridPos.x != 43 && mouseGridPos.y == 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
			}
			else if (mouseGridPos.x != 0 && mouseGridPos.x == 43 && mouseGridPos.y == 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
			}
			else
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
			}
		}
		if (mouseX > 12 && mouseY > 12)
		{
			if (mouseGridPos.x == 43 && mouseGridPos.y == 0)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
			}
			else if (mouseGridPos.x == 0 && mouseGridPos.y == 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
			}
			else if (mouseGridPos.x == 43 && mouseGridPos.y != 0 && mouseGridPos.y != 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, 24);
			}
			else if (mouseGridPos.x == 43 && mouseGridPos.y == 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(-24, -24);
			}
			else if (mouseGridPos.x != 43 && mouseGridPos.x != 0 && mouseGridPos.y == 39)
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, -24);
			}
			else
			{
				selectorCursor[3]->getComponent<TransformComponent>().position = (mouseGridPos * 24) + Vec2(24, 24);
			}
		}

		int index = 0;

		for (std::shared_ptr<Entity> e : selectorCursor)
		{
			if (!grid[e->getComponent<TransformComponent>().position.y / 24][e->getComponent<TransformComponent>().position.x / 24].buildable)
			{
				e->getComponent<SpriteComponent>().srcRect.x = 32;
				cursors[index] = false;
			}
			else
			{
				e->getComponent<SpriteComponent>().srcRect.x = 0;
				cursors[index] = true;
				cursorlocation[index] = e->getComponent<TransformComponent>().position;
			}
			index++;
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

	buyTowerButton = m_entityManager.addEntity("Button");
	buyTowerButton->addComponent<TransformComponent>(Vec2(1068, 856), Vec2(0, 0), 0);
	buyTowerButton->addComponent<SpriteComponent>("buybutton.png", 200, 100, 1, false);
	buyTowerButton->addComponent<ButtonComponent>();
	buyTowerButton->getComponent<ButtonComponent>().isActive = true;

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
	dealBackground = m_entityManager.addEntity("DealBackground");
	dealBackground->addComponent<TransformComponent>(Vec2(0, 0), Vec2(0, 0), 0);
	dealBackground->addComponent<SpriteComponent>("dealbackground.png", 1280, 700, 1, false);
	dealBackground->getComponent<SpriteComponent>().isActive = false;
}

void Scene_Poker::createCursor()
{
	for (int i = 0; i < 4; i++)
	{
		selectorCursor[i] = m_entityManager.addEntity("SelectorCursor");
		selectorCursor[i]->addComponent<TransformComponent>(Vec2(0, 0), Vec2(0, 0), 0);
		selectorCursor[i]->addComponent<SpriteComponent>("selector.png", 32, 32, 0.75, false);
		selectorCursor[i]->getComponent<SpriteComponent>().isActive = false;
	}
}

void Scene_Poker::createRefreshButton()
{
	for (int i = 0; i < 5; i++)
	{
		refreshButton[i] = m_entityManager.addEntity("Button");
		refreshButton[i]->addComponent<SpriteComponent>("refresh.png", 150, 64, 1, false);
		refreshButton[i]->addComponent<TransformComponent>(Vec2(115 + (i * 225), 575), Vec2(0, 0), 0);
		refreshButton[i]->addComponent<ButtonComponent>();
		refreshButton[i]->getComponent<ButtonComponent>().isActive = false;
		refreshButton[i]->getComponent<SpriteComponent>().isActive = false;
	}
}

void Scene_Poker::createAcceptButton()
{
	acceptButton = m_entityManager.addEntity("Button");
	acceptButton->addComponent<SpriteComponent>("accept.png", 100, 100, 1, false);
	acceptButton->addComponent<TransformComponent>(Vec2(1150, 30), Vec2(0, 0), 0);
	acceptButton->addComponent<ButtonComponent>();
	acceptButton->getComponent<ButtonComponent>().isActive = false;
	acceptButton->getComponent<SpriteComponent>().isActive = false;
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
		grid[cursorlocation[i].y / 24][cursorlocation[i].x / 24].cell = tower;
	}

	tower->addComponent<TransformComponent>(LowestPosition, Vec2(0, 0), 0);
	tower->addComponent<SpriteComponent>("tower.png", 48, 48, 1, false);
	tower->getComponent<SpriteComponent>().isActive = true;

	towerList.push_back(tower);

	/*tower->addComponent<TowerComponent>();
	tower->getComponent<TowerComponent>().level = 1;
	tower->getComponent<TowerComponent>().damage = 10;
	tower->getComponent<TowerComponent>().range = 100;
	tower->getComponent<TowerComponent>().attackSpeed = 1.0f;
	tower->getComponent<TowerComponent>().cost = 100;
	*/
}

void Scene_Poker::dealHand()
{
	for (int i = 0; i < 5; i++)
	{
		m_playerHand[i] = m_entityManager.addEntity("Card");
		int randrank = 0 + std::rand() % (7 - 0 + 1);
		CardComponent::Rank rank = static_cast<CardComponent::Rank>(randrank);
		int randsuit = 0 + std::rand() % (2 - 0 + 1);
		CardComponent::Suit suit = static_cast<CardComponent::Suit>(randsuit);
		m_playerHand[i]->addComponent<CardComponent>(rank, suit);
		m_playerHand[i]->addComponent<SpriteComponent>("card.png", 100, 175, 2, false);
		m_playerHand[i]->addComponent<TransformComponent>(Vec2(90+(i*225), 200), Vec2(0, 0), 0);
		createface(m_playerHand[i]);
	}
}

void Scene_Poker::clearHand()
{
	for (int i = 0; i < 5; i++)
	{
		m_playerHand[i]->destroy();
	}
}

void Scene_Poker::handResult()
{
	int index = 0;

	for (auto& e : m_playerHand)
	{
		m_sortHand[index] = e;
		index++;
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
	setTowerStats(result.second, highCard, towerList.back());
	handresult = true;
}

void Scene_Poker::changeCard(std::shared_ptr<Entity>* card, int selection)
{
	int randrank = 0 + std::rand() % (7 - 0 + 1);
	CardComponent::Rank newrank = static_cast<CardComponent::Rank>(randrank);
	int randsuit = 0 + std::rand() % (2 - 0 + 1);
	CardComponent::Suit newsuit = static_cast<CardComponent::Suit>(randsuit);
	card[selection]->getComponent<CardComponent>().rank = newrank;
	card[selection]->getComponent<CardComponent>().suit = newsuit;
	createface(card[selection]);
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
	if (handText)
	{
		handText->destroy();
	}
	handText = m_entityManager.addEntity("Text");
	handText->addComponent<TextComponent>(s, "Snes.ttf", size, 700, 300, 1, SDL_Color{ 255, 255, 255 });
	handText->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
}

void Scene_Poker::createTimerText(std::string s, int size, Vec2 pos)
{
	if (timerText)
	{
		timerText->destroy();
	}
	timerText = m_entityManager.addEntity("Text");
	timerText->addComponent<TextComponent>(s, "Snes.ttf", size, 700, 300, 1, SDL_Color{ 255, 255, 255 });
	timerText->addComponent<TransformComponent>(pos, Vec2(0, 0), 0);
}

void Scene_Poker::setTowerStats(int hand, int high, std::shared_ptr<Entity> tower)
{
	tower->addComponent<TowerComponent>(hand, high, 4, 0.33f);
}

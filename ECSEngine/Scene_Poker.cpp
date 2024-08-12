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
		if (action.getName() == "DEAL")
		{
			clearHand();
			dealHand();
			sPoker.sortHand(m_playerHand);
			showHand();
			int result = sPoker.checkHand(m_playerHand);
			int resultt = sPoker.checkHighCard(m_playerHand, result);
			switch (resultt)
			{
				case 0:
					std::cout << "Seven High" << std::endl;
					break;
				case 1:
					std::cout << "Eight High" << std::endl;
					break;
				case 2:
					std::cout << "Nine High" << std::endl;
					break;
				case 3:
					std::cout << "Ten High" << std::endl;
					break;
				case 4:
					std::cout << "Jack High" << std::endl;
					break;
				case 5:
					std::cout << "Queen High" << std::endl;
					break;
				case 6:
					std::cout << "King High" << std::endl;
					break;
				case 7:
					std::cout << "Ace High" << std::endl;
					break;
			}
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
			auto& sprite = e->getComponent<SpriteComponent>();
			auto& transform = e->getComponent<TransformComponent>();

			sprite.destRect.x = static_cast<int>(transform.position.x);
			sprite.destRect.y = static_cast<int>(transform.position.y);
			sprite.destRect.w = sprite.width * sprite.scale;
			sprite.destRect.h = sprite.height * sprite.scale;
			m_graphics->DrawTexture(sprite.texture, &sprite.srcRect, &sprite.destRect, transform.angle);
		}
	}
}

void Scene_Poker::update()
{
	m_entityManager.update();
}

void Scene_Poker::init()
{
	m_title = "POKER";

	registerAction(SDL_SCANCODE_ESCAPE, "QUIT");
	registerAction(SDL_SCANCODE_RETURN, "DEAL");
	registerAction(SDL_SCANCODE_1, "FIRST");
	registerAction(SDL_SCANCODE_2, "SECOND");
	registerAction(SDL_SCANCODE_3, "THIRD");
	registerAction(SDL_SCANCODE_4, "FOURTH");
	registerAction(SDL_SCANCODE_5, "FIFTH");

	m_entityManager = EntityManager();

	m_timer = Timer::Instance();

	std::srand(std::time(nullptr));

	
	//dealHand();
	//sPoker.sortHand(m_playerHand);
	//showHand();
	//int result = sPoker.checkHand(m_playerHand);
}

void Scene_Poker::onEnd()
{
	m_hasEnded = true;
	m_game->changeScene("MENU", nullptr, true);
}

void Scene_Poker::createUI()
{
	createBackground();
	buyTowerButton = m_entityManager.addEntity("UI");
	buyTowerButton->addComponent<TransformComponent>();
	buyTowerButton->addComponent<SpriteComponent>();
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
			}
		}
		myfile.close();
	}
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

void Scene_Poker::showHand()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_playerHand[i]->getComponent<CardComponent>().rank == 0)
		{
			std::cout << "Seven";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 1)
		{
			std::cout << "Eight";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 2)
		{
			std::cout << "Nine";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 3)
		{
			std::cout << "Ten";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 4)
		{
			std::cout << "Jack";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 5)
		{
			std::cout << "Queen";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 6)
		{
			std::cout << "King";
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().rank == 7)
		{
			std::cout << "Ace";
		}
		
		if (m_playerHand[i]->getComponent<CardComponent>().suit == 0)
		{
			std::cout << " Red" << std::endl;
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().suit == 1)
		{
			std::cout << " Black" << std::endl;
		}
		else if (m_playerHand[i]->getComponent<CardComponent>().suit == 2)
		{
			std::cout << " White" << std::endl;
		}
	}
}

void Scene_Poker::clearHand()
{
	for (int i = 0; i < 5; i++)
	{
		m_playerHand[i]->destroy();
	}
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
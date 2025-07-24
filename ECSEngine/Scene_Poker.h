#pragma once
#include "Scene.h"
#include "Timer.h"
#include "Poker.h"
#include "Physics.h"

class Scene_Poker : public Scene
{

public:
	Scene_Poker();
	Scene_Poker(GameManager* game);
	void sDoAction(const Action& action) override;
	void sRender() override;
	void update() override;

private:

	std::string m_title;

	Timer* m_timer = nullptr;

	Physics mPhysics;

	float elapseTime = 0;

	enum class State { BUILD, DEAL, WAVE };

	struct GridCell
	{
		std::shared_ptr<Entity> cell;

		GridCell() :  cell(nullptr) {};
		GridCell(std::shared_ptr<Entity> ent) : cell(ent) {};
	};

	std::vector<std::vector<GridCell>> grid;

	State m_state = State::BUILD;

	std::shared_ptr<Entity> m_playerHand[5];
	Poker sPoker;

	std::shared_ptr<Entity> buyTowerButton;
	std::shared_ptr<Entity> selectorCursor;

	bool isBuying = false;

	void init();
	void onEnd();

	void sUICollision(std::shared_ptr<Entity> object);

	void addTile(int x, int y, std::shared_ptr<Entity> tile);
	void selectTiles(Vec2 vec);

	void createUI();
	void createBackground();
	void createCursor();
	
	void dealHand();
	void showHand();
	void clearHand();

	void changeCard(std::shared_ptr<Entity>* card, int selection);

	void createface(std::shared_ptr<Entity> card);


};


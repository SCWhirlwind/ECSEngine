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

	void renderEntities(std::shared_ptr<Entity> entity);

	float elapseTime = 0;

	enum class State { BUILD, DEAL, WAVE };

	struct GridCell
	{
		std::shared_ptr<Entity> cell;

		bool buildable = false;

		GridCell() :  cell(nullptr) {};
		GridCell(std::shared_ptr<Entity> ent) : cell(ent) {};
	};

	std::vector<std::vector<GridCell>> grid;

	State m_state = State::BUILD;

	std::vector<std::shared_ptr<Entity>> towerList;

	std::shared_ptr<Entity> m_playerHand[5];
	std::shared_ptr<Entity> m_sortHand[5];
	Poker sPoker;

	std::shared_ptr<Entity> buyTowerButton;

	std::shared_ptr<Entity> selectorCursor[4];
	Vec2 cursorlocation[4] = {	Vec2(0, 0), Vec2(0, 0), Vec2(0, 0), Vec2(0, 0) };

	std::shared_ptr<Entity> dealBackground;

	std::shared_ptr<Entity> refreshButton[5];

	std::shared_ptr<Entity> acceptButton;

	std::shared_ptr<Entity> handText;
	std::shared_ptr<Entity> timerText;

	float currentTime = 5.0f;
	int previousTime = 0;

	bool handresult = false;

	bool dealt = false;

	bool isBuying = false;

	void init();
	void onEnd();

	void sUICollision(std::shared_ptr<Entity> object);

	void addTile(int x, int y, std::shared_ptr<Entity> tile);
	bool selectTiles();

	void createUI();
	void createBackground();
	void createDealBackground();
	void createCursor();
	void createRefreshButton();
	void createAcceptButton();

	void createTower();
	
	void dealHand();
	void clearHand();

	void handResult();

	void changeCard(std::shared_ptr<Entity>* card, int selection);

	void createface(std::shared_ptr<Entity> card);

	void createHandText(std::string s, int size, Vec2 pos);
	void createTimerText(std::string s, int size, Vec2 pos);	

	void setTowerStats(int hand, int high, std::shared_ptr<Entity> tower);
};


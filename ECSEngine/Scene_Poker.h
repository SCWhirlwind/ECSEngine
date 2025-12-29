#pragma once
#include "Scene.h"
#include "Timer.h"
#include "Poker.h"
#include "Physics.h"

using EntityID = int;

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

	enum class State { TEST, BUILD, DEAL, WAVE };

	struct GridCell
	{
		EntityID cell;

		bool buildable = false;

		GridCell() :  cell(0) {};
		GridCell(EntityID entid) : cell(entid) {};
	};

	struct WaveData
	{
		int count;
		int hp;
		int speed;
		int index;
	};

	std::vector<WaveData> waves;

	std::vector<std::vector<GridCell>> grid;

	State m_state = State::BUILD;

	std::vector<EntityID> towerList;

	EntityID m_playerHand[5];
	std::shared_ptr<Entity> m_sortHand[5];
	Poker sPoker;

	EntityID buyTowerButton;

	EntityID selectorCursor[4];
	Vec2 cursorlocation[4] = {	Vec2(0, 0), Vec2(0, 0), Vec2(0, 0), Vec2(0, 0) };

	EntityID dealBackground;

	EntityID refreshButton[5];

	EntityID acceptButton;

	EntityID handText;
	EntityID timerText;

	EntityID spawnPoint;

	std::vector<EntityID> waypoints;

	std::vector<EntityID> enemyList;

	std::vector<EntityID> projectiles;

	float currentTime = 5.0f;
	int previousTime = 0;

	float timeGap = 0.75f;
	float timer = 0.0f;

	int level = 0;
	int enemiesSpawned = 0;
	int enemiesPerWave = 0;
	int enemyHP = 0;
	int enemySpeed = 0;
	int index = 0;

	bool spawningDone = false;
	int aliveEnemies = 0;

	bool handresult = false;

	bool dealt = false;
	float dealDelay = 1.0f;
	float dealTimer = 0.0f;

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

	void loadWaveData();

	void createTower();
	
	void dealHand();
	void clearHand();

	void handResult();

	void changeCard(std::shared_ptr<Entity> card, int selection);

	void createface(std::shared_ptr<Entity> card);

	void createHandText(std::string s, int size, Vec2 pos);
	void createTimerText(std::string s, int size, Vec2 pos);	

	void setTowerStats(int hand, int high, std::shared_ptr<Entity> tower);

	EntityID createPoints(Vec2 p);

	void createWaypoint();

	void createEnemy(Vec2 p, int hp, int speed, int index);

	void createProjectile(Vec2 p, int damage, std::shared_ptr<Entity> target, int speed);

	void updateEnemiesInRange(std::shared_ptr<Entity> enemy);

	void towerAttack(std::shared_ptr<Entity> tower);

	void resetLoop();
};


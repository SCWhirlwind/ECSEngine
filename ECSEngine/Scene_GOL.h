#pragma once
#include "Scene.h"
#include <unordered_map>
#include "Timer.h"

class Scene_GOL : public Scene
{
public:
	Scene_GOL();
	Scene_GOL(GameManager* game);
	void sDoAction(const Action& action) override;
	void sRender() override;
	void update() override;

private:

	std::string m_title;

	Timer* m_timer = nullptr;

	float timer = 0;

	std::shared_ptr<Entity> golArray[128][160];

	bool toggle = false;

	float speed = 1;

	void init();
	void onEnd();

	void createGrid();
	std::shared_ptr<Entity> createTile(int row, int height);

	void setCurrentToPrev();
	int checkNeighbors(int row, int column);
	void life();
	void randomize();
};


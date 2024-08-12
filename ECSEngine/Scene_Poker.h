#pragma once
#include "Scene.h"
#include "Timer.h"
#include "Poker.h"

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

	float elapseTime = 0;

	std::shared_ptr<Entity> m_playerHand[5];
	Poker sPoker;

	std::shared_ptr<Entity> buyTowerButton;

	void init();
	void onEnd();

	void createUI();
	void createBackground();
	
	void dealHand();
	void showHand();
	void clearHand();

	void changeCard(std::shared_ptr<Entity>* card, int selection);

	void createface(std::shared_ptr<Entity> card);


};


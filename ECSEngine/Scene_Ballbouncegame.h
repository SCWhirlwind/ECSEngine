#pragma once
#include "Scene.h"
#include "Physics.h"
#include "Timer.h"

class Scene_Ballbouncegame : public Scene
{
public:

	Scene_Ballbouncegame();
	Scene_Ballbouncegame(GameManager* game);
	void sDoAction(const Action& action) override;
	void sRender() override;
	void update() override;

private:

	std::string m_title;

	Timer* m_timer = nullptr;

	Physics mPhysics;

	std::shared_ptr<Entity> m_player;

	bool hit = false;

	int score = 0;
	int highscore = 0;

	std::shared_ptr<Entity> scorelabel;

	std::shared_ptr<Entity> hiscorelabel;
	float gravity = 0.04f;

	float ballspawnrate = 0.15f;

	float timer = 0;

	int maxVel = 8;

	void init();
	void onEnd();

	void sCollision();
	void CreateScore(int size, Vec2 pos);
	void CreateHiScore(int size, Vec2 pos);
	void HiScore();
	void UpdateScore();

	void spawnBall(float speed, int height, int size);

	void spawnPlayer();
};


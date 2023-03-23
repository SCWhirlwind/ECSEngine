#pragma once
#include "Scene.h"
#include "Timer.h"

class Scene_Spawn : public Scene
{
public:
    Scene_Spawn();
    Scene_Spawn(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    std::string m_title;

    Timer* m_timer = nullptr;

    float elapseTime = 0;

    float spawn = 1.0f;

    float yOffset = 0.0f;

    std::shared_ptr<Entity> m_spawner;

    void init();
    void onEnd();

    void createSpawner();

    void createBullet();

    void spawnInterval(float f);

    void moveSpawner(int v);
};


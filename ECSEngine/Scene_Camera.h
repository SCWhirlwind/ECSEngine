#pragma once
#include "Scene.h"
#include "Physics.h"
class Scene_Camera : public Scene
{
public:

    const int LEVEL_WIDTH = 3200;
    const int LEVEL_HEIGHT = 3200;

    Scene_Camera();
    Scene_Camera(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    std::string m_title;

    SDL_Rect* camera;

    std::shared_ptr<Entity> m_player;

    Physics m_physics;

    void init();
    void onEnd();

    void createGridBackground();
    void spawnTile(Vec2 vec);

    void spawnPlayer();
};


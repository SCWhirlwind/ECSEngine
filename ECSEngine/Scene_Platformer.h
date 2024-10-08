#pragma once
#include "Scene.h"
#include "Physics.h"
#include <unordered_map>

class Scene_Platformer : public Scene
{
public:

    Scene_Platformer();
    Scene_Platformer(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    //const int LEVEL_WIDTH = 10240;
    const int LEVEL_WIDTH = 3200;
    const int LEVEL_HEIGHT = 1024;

    const int TILE_WIDTH = 32;
    const int TILE_HEIGHT = 32;

    const int SCALE = 2;

    const int SCALED_TILE_WIDTH = TILE_WIDTH * SCALE;
    const int SCALED_TILE_HEIGHT = TILE_HEIGHT * SCALE;

    const int GRID_WIDTH = LEVEL_WIDTH / SCALED_TILE_WIDTH;
    const int GRID_HEIGHT = LEVEL_HEIGHT / SCALED_TILE_HEIGHT;

    const float gravityVelocity = 0.3f;

    std::string m_title;

    SDL_FRect camera;

    bool canJump = true;

    bool renderTexture = true;
    bool renderCollision = false;

    std::shared_ptr<Entity> m_player;

    std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>> cell_map;

    Physics m_physics;

    void init();
    void onEnd();

    void sPlayerUpdate();
    void sCamera();
    void sCollision();

    void createFloor();

    void addToBucket(Vec2 pos, std::shared_ptr<Entity> entity);

    void removeFromBucket(Vec2 pos, std::shared_ptr<Entity> entity);

    std::string getPosKey(Vec2 pos);

    std::shared_ptr<Entity> createEntity(Vec2 pos, std::string name);

    bool removeEntity(Vec2 pos, std::string name);

    void rayCast(Vec2 a);
    Vec2 lineIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
};


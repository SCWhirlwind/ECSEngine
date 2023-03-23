#pragma once
#include "Scene.h"

class Scene_Grid : public Scene
{

public:

    Scene_Grid();
    Scene_Grid(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    const int rows = 32;
    const int cols = 32;

    std::string m_title;

    std::shared_ptr<Entity> m_Object;

    std::shared_ptr<Entity> m_Player;
    std::shared_ptr<Entity> m_PrevHover;

    std::vector<std::vector<std::shared_ptr<Entity>>> m_tileArray;

    void init();
    void onEnd();

    std::shared_ptr<Entity> spawnTile(Vec2 vec);

    void createGrid();

    void createIsoGrid();

    void checkMousePress();

    void tileHover();

    std::shared_ptr<Entity> grabTile();

    Vec2 gridCoord();
};


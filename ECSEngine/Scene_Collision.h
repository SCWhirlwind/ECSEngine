#pragma once
#include "Scene.h"

class Scene_Collision : public Scene
{
public:

    Scene_Collision();
    Scene_Collision(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    void init();
    void onEnd();
};


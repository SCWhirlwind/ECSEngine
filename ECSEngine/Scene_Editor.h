#pragma once
#include "Scene.h"

class Scene_Editor : public Scene
{
public:

    Scene_Editor();
    Scene_Editor(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    void init();
    void onEnd();
};


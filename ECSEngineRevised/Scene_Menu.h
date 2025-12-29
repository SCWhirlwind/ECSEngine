#pragma once
#include "Scene.h"

class Scene_Menu : public Scene
{
public:

    Scene_Menu();
    Scene_Menu(GameManager* game);
    void sDoAction(const Action& action) override;
    void sRender() override;
    void update() override;

private:

    std::string m_title;

    std::shared_ptr<Entity> cursor;

    int m_menuIndex = 0;

    void init();
    void onEnd();

    void createText(std::string s, int size, Vec2 pos);
    void createCursor();
    void moveCursor(int i);
};
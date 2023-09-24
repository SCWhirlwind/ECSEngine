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

    std::string m_title, submenu_heighttextinput = "16";

    std::shared_ptr<Entity> cursor, submenu, submenu_widthtext, submenu_heighttext, submenu_heighttextbox, submenu_widthtextbox, submenu_heighttextinputentity;

    int m_menuIndex = 0;

    int state = MENU;

    bool isInput = false;

    enum STATE { MENU, NEW, LOAD };

    SDL_FRect camera;

    void init();
    void onEnd();

    void createText(std::string s, int size, Vec2 pos);
    void createCursor();
    void createSubMenu();
    void moveCursor(int i);

    void heightText(std::string s);
};


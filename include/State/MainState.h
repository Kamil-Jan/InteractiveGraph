#pragma once
#include <stack>
#include <unordered_map>
#include <Graph/Graph.h>
#include <Graph/Vertex.h>
#include <State/State.h>
#include <Mode/Mode.h>
#include <GUI/Widget/Button.h>
#include <GUI/Menu/TopMenu.h>
#include <GUI/Menu/BottomMenu.h>

using std::stack;
using std::unordered_map;

class MainState: public State
{
    private:
        static MainState* instance;

        Mode* currentMode;
        TopMenu* mainMenu;
        BottomMenu* bottomMenu;

    public:
        MainState();

        static MainState* getInstance();
        TopMenu* getMainMenu();
        BottomMenu* getBottomMenu();
        Mode* getCurrentMode();
        void setCurrentMode(Mode* mode);

        bool isAroundMenus(int x, int y);
        bool isInsideMenusWidgets(int x, int y);

        vector<Vertex*> getSpritesUnderMouse(int mouseX, int mouseY);
        void removeVertex(Vertex* vertex);

        void update();
        void render();
};


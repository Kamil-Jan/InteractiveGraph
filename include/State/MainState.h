#pragma once
#include <stack>
#include <unordered_map>
#include <Graph/Graph.h>
#include <Graph/Vertex.h>
#include <State/State.h>
#include <Mode/Mode.h>
#include <GUI/Widget/Button.h>
#include <GUI/Menu/MainMenu.h>

using std::stack;
using std::unordered_map;

class MainState: public State
{
    private:
        static MainState* instance;

        Mode* currentMode;
        MainMenu* mainMenu;

    public:
        MainState();

        static MainState* getInstance();
        MainMenu* getMainMenu();
        Mode* getCurrentMode();
        void setCurrentMode(Mode* mode);

        vector<Vertex*> getSpritesUnderMouse(int mouseX, int mouseY);
        void removeVertex(Vertex* vertex);

        void update();
        void render();
};


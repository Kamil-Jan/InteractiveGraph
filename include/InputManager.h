#pragma once
#include <unordered_map>

using std::unordered_map;

class InputManager
{
    private:
        static InputManager* instance;
        unordered_map<int, bool> keyState;
        unordered_map<int, int> keyUpdate;

        bool mouseState[6] = {false, false, false, false, false, false};
        int mouseUpdate[6];

        int updateCounter;
        bool mQuitRequested;

    public:
        InputManager();

        static InputManager* getInstance();
        bool quitRequested();

        void update();
        bool keyPressed(int key);
        bool keyReleased(int key);
        bool isKeyDown(int key);

        bool mousePressed(int button);
        bool mouseReleased(int button);
        bool isMouseDown(int button);
};


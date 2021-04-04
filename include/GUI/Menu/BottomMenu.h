#pragma once
#include <string>
#include <GUI/Menu/Menu.h>

class BottomMenu : public Menu
{
    public:
        BottomMenu(int x, int y, int width, int height,
                   int borderWidth, vector<int> bgColor,
                   vector<int> borderColor);
};


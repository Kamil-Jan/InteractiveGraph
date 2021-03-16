#pragma once
#include <string>
#include <GUI/Menu/Menu.h>

class BottomMenu : public Menu
{
    private:
        std::string description;

    public:
        BottomMenu(int x, int y, int width, int height,
                   int borderWidth, vector<int> bgColor,
                   vector<int> borderColor);
        std::string getDescription();
        void setDescription(std::string descr);
};


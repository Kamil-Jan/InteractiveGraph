#pragma once
#include <string>
#include <GUI/Menu/Menu.h>
#include <GUI/Widget/Dropdown.h>
#include <GUI/Widget/PressedButton.h>

class TopMenu : public Menu
{
    private:
        Dropdown* dropdown;
        PressedButton* drawGraphButton;

    public:
        TopMenu(int x, int y, int width, int height,
                 int borderWidth, vector<int> bgColor,
                 vector<int> borderColor);
        std::string getSelectedAlgorithm();
};


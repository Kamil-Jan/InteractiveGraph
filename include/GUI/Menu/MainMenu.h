#pragma once
#include <string>
#include <GUI/Menu/Menu.h>
#include <GUI/Widget/Dropdown.h>
#include <GUI/Widget/PressedButton.h>

class MainMenu : public Menu
{
    private:
        Dropdown* dropdown;
        PressedButton* drawGraphButton;

    public:
        MainMenu(int x, int y, int width, int height,
                 int borderWidth, vector<int> bgColor,
                 vector<int> borderColor);
        std::string getSelectedAlgorithm();
        void resetWidgets();
        void update(float ticks) override;
};


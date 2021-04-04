#include <GUI/Menu/BottomMenu.h>
#include <GUI/Widget/PressedButton.h>
#include <Action/SetDirectedAction.h>
#include <Action/SetUndirectedAction.h>
#include <Game.h>

BottomMenu::BottomMenu(int x, int y, int width, int height,
                       int borderWidth, vector<int> bgColor,
                       vector<int> borderColor) :
    Menu(x, y, width, height, borderWidth, bgColor, borderColor)
{
    int padding = 15;
    int buttonWidth = 175;
    int buttonHeight = getHeight() - padding;

    PressedButton* pressedButton = new PressedButton(
        "Directed",
        new SetDirectedAction(),
        getX() + padding/2,
        getY() + padding/2,
        buttonWidth, buttonHeight,
        Game::BUTTON_BORDER_WIDTH,
        Game::BUTTON_BG_COLOR,
        Game::BUTTON_SELECTED_BG_COLOR,
        Game::BUTTON_BORDER_COLOR
    );
    pressedButton->setSelected();
    addWidget(pressedButton);

    addWidget(
        new PressedButton(
            "Undirected",
            new SetUndirectedAction(),
            getX() + 1.5*padding + buttonWidth,
            getY() + padding/2,
            buttonWidth, buttonHeight,
            Game::BUTTON_BORDER_WIDTH,
            Game::BUTTON_BG_COLOR,
            Game::BUTTON_SELECTED_BG_COLOR,
            Game::BUTTON_BORDER_COLOR
        )
    );
}


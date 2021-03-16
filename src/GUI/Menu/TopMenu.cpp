#include <GUI/Menu/TopMenu.h>
#include <GUI/Widget/Button.h>
#include <GUI/Widget/RoundButton.h>
#include <Action/ClearVerticesAction.h>
#include <Action/PushStateAction.h>
#include <Action/SetModeAction.h>
#include <State/HelpState.h>
#include <Mode/ShortestPathMode.h>
#include <Mode/ArticulationPointsMode.h>
#include <Mode/DeleteMode.h>
#include <Mode/DrawMode.h>
#include <Mode/EditMode.h>
#include <Logger.h>
#include <Game.h>

TopMenu::TopMenu(int x, int y, int width, int height,
                   int borderWidth, vector<int> bgColor,
                   vector<int> borderColor) :
    Menu(x, y, width, height, borderWidth, bgColor, borderColor)
{
    Logger::debug("MainMenu::MainMenu");
    int padding = 15;
    int buttonWidth = 175;
    int buttonHeight = getHeight() - padding;

    int dropdownWidth = 275;
    int widgetsShift = dropdownWidth - buttonWidth;
    dropdown = new Dropdown(
        { { "Shortest path", new SetModeAction(new ShortestPathMode()) },
          { "Cut vertices", new SetModeAction(new ArticulationPointsMode()) }},
        "Select algorithm",
        getX() + padding/2,
        getY() + padding/2,
        dropdownWidth, buttonHeight,
        Game::BUTTON_BORDER_WIDTH,
        Game::BUTTON_BG_COLOR,
        Game::MENU_BG_COLOR,
        Game::BUTTON_SELECTED_BG_COLOR,
        Game::BUTTON_BORDER_COLOR
    );
    addWidget(dropdown);

    drawGraphButton = new PressedButton(
        "Draw",
        new SetModeAction(new DrawMode()),
        getX() + 1.5*padding + buttonWidth + widgetsShift,
        getY() + padding/2,
        buttonWidth, buttonHeight,
        Game::BUTTON_BORDER_WIDTH,
        Game::BUTTON_BG_COLOR,
        Game::BUTTON_SELECTED_BG_COLOR,
        Game::BUTTON_BORDER_COLOR
    );
    drawGraphButton->setSelected();
    addWidget(drawGraphButton);

    addWidget(
        new PressedButton(
            "Edit",
            new SetModeAction(new EditMode()),
            getX() + 2.5*padding + 2*buttonWidth + widgetsShift,
            getY() + padding/2,
            buttonWidth, buttonHeight,
            Game::BUTTON_BORDER_WIDTH,
            Game::BUTTON_BG_COLOR,
            Game::BUTTON_SELECTED_BG_COLOR,
            Game::BUTTON_BORDER_COLOR
        )
    );

    addWidget(
        new PressedButton(
            "Delete",
            new SetModeAction(new DeleteMode()),
            getX() + 3.5*padding + 3*buttonWidth + widgetsShift,
            getY() + padding/2,
            buttonWidth, buttonHeight,
            Game::BUTTON_BORDER_WIDTH,
            Game::BUTTON_BG_COLOR,
            Game::BUTTON_SELECTED_BG_COLOR,
            Game::BUTTON_BORDER_COLOR
        )
    );

    addWidget(
        new Button(
            "Clear",
            new ClearVerticesAction(),
            getX() + 4.5*padding + 4*buttonWidth + widgetsShift,
            getY() + padding/2,
            buttonWidth, buttonHeight,
            Game::BUTTON_BORDER_WIDTH,
            Game::BUTTON_BG_COLOR,
            Game::BUTTON_SELECTED_BG_COLOR,
            Game::BUTTON_BORDER_COLOR
        )
    );
}

std::string TopMenu::getSelectedAlgorithm()
{
    Logger::debug("MainMenu::getSelectedAlgorithm");
    return dropdown->getSelectedItem();
}


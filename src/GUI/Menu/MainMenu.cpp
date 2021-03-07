#include <GUI/Menu/MainMenu.h>
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

MainMenu::MainMenu(int x, int y, int width, int height,
                   int borderWidth, vector<int> bgColor,
                   vector<int> borderColor) :
    Menu(x, y, width, height, borderWidth, bgColor, borderColor)
{
    Logger::debug("MainMenu::MainMenu");
    int padding = 15;
    int buttonWidth = 200;
    int buttonHeight = getHeight() - padding;

    dropdown = new Dropdown(
        { { "Shortest path", new SetModeAction(new ShortestPathMode()) },
          { "Cut vertices", new SetModeAction(new ArticulationPointsMode()) }},
        "Select algorithm",
        getX() + padding/2,
        getY() + padding/2,
        300, buttonHeight,
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
        getX() + 1.5*padding + buttonWidth + 100,
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
            getX() + 2.5*padding + 2*buttonWidth + 100,
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
            getX() + 3.5*padding + 3*buttonWidth + 100,
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
            getX() + 4.5*padding + 4*buttonWidth + 100,
            getY() + padding/2,
            buttonWidth, buttonHeight,
            Game::BUTTON_BORDER_WIDTH,
            Game::BUTTON_BG_COLOR,
            Game::BUTTON_SELECTED_BG_COLOR,
            Game::BUTTON_BORDER_COLOR
        )
    );

    State* helpState = new HelpState();
    SDL_Texture* questionMarkTex = IMG_LoadTexture(Game::getInstance()->getRenderer(), "res/question_mark.png");

    int radius = 0.4*getHeight();
    addWidget(
        new RoundButton(
            questionMarkTex,
            new PushStateAction(helpState),
            getWidth() - radius - padding/2,
            getHeight() / 2,
            radius, 0.75*Game::BUTTON_BORDER_WIDTH,
            Game::BUTTON_BG_COLOR,
            Game::BUTTON_SELECTED_BG_COLOR,
            Game::BUTTON_BORDER_COLOR
        )
    );
}

std::string MainMenu::getSelectedAlgorithm()
{
    Logger::debug("MainMenu::getSelectedAlgorithm");
    return dropdown->getSelectedItem();
}

void MainMenu::resetWidgets()
{
    Logger::debug("MainMenu::resetWidgets");
    for (Widget* widget : widgets.getSprites())
        widget->reset();
}

void MainMenu::update(float ticks)
{
    Logger::debug("MainMenu::update");
    Menu::update(ticks);
}


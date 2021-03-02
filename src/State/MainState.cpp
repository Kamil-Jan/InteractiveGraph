#include <iostream>
#include <deque>
#include <SDL2/SDL.h>

#include <Game.h>
#include <Logger.h>
#include <InputManager.h>
#include <State/MainState.h>
#include <State/HelpState.h>
#include <GUI/Menu/MainMenu.h>
#include <Action/PushStateAction.h>
#include <Mode/DrawMode.h>
#include <Mode/ShortestPathMode.h>

MainState* MainState::instance = NULL;

MainState::MainState() : State()
{
    Logger::debug("MainState::MainState");
    instance = instance? instance : this;

    mainMenu = new MainMenu(
        0, 0, SCREEN_WIDTH, 60,
        Game::MENU_BORDER_WIDTH,
        Game::MENU_BG_COLOR,
        Game::MENU_BORDER_COLOR
    );

    currentMode = new DrawMode();
}

MainState* MainState::getInstance()
{
    return instance;
}

MainMenu* MainState::getMainMenu()
{
    return mainMenu;
}

Mode* MainState::getCurrentMode()
{
    return currentMode;
}

void MainState::setCurrentMode(Mode* mode)
{
    currentMode = mode;
}

vector<Vertex*> MainState::getSpritesUnderMouse(int mouseX, int mouseY)
{
    Logger::debug("MainState::getSpritesUnderMouse");
    std::deque<Vertex*> spritesUnderMouse;
    for (Vertex* vertex : Graph::getInstance()->getVertices()) {
        if (vertex->isInside(mouseX, mouseY)) {
            spritesUnderMouse.push_front(vertex);
        }
        vertex->resetSelected();
    }
    return { spritesUnderMouse.begin(), spritesUnderMouse.end() };
}

void MainState::update()
{
    Logger::debug("MainState::update started");

    if (currentMode) currentMode->update(SDL_GetTicks());
    Graph::getInstance()->update(SDL_GetTicks());
    mainMenu->update(SDL_GetTicks());
}

void MainState::render()
{
    Logger::debug("MainState::render");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();

    vector<int> bgColor = Game::BG_COLOR;
    SDL_SetRenderDrawColor(renderer, bgColor[0],
                           bgColor[1], bgColor[2],
                           bgColor[3]);
    SDL_RenderClear(renderer);

    Graph::getInstance()->render();
    if (currentMode) currentMode->render();
    mainMenu->render();
}


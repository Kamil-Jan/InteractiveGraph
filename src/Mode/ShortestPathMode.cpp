#include <iostream>
#include <Mode/ShortestPathMode.h>
#include <GUI/Widget/Button.h>
#include <Action/BackAction.h>
#include <Action/ClearPathAction.h>
#include <State/MainState.h>
#include <InputManager.h>
#include <Logger.h>
#include <Game.h>

ShortestPathMode* ShortestPathMode::instance = NULL;

ShortestPathMode::ShortestPathMode() : Mode()
{
    Logger::debug("ShortestPathMode::ShortestPathMode");
    instance = instance? instance : this;
}

ShortestPathMode* ShortestPathMode::getInstance()
{
    return instance;
}

void ShortestPathMode::resetSelectedVertex()
{
    if (selectedVertex != NULL)
        selectedVertex->resetSelected();
    selectedVertex = NULL;
}

void ShortestPathMode::resetPath()
{
    for (Vertex* vertex : path) vertex->resetSelected();
    path.clear();
}

void ShortestPathMode::update(float ticks)
{
    Logger::debug("ShortestPathMode::update");
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MainState* mainState = MainState::getInstance();
    vector<Vertex*> spritesUnderMouse = mainState->getSpritesUnderMouse(mouseX, mouseY);

    if (!spritesUnderMouse.empty())
        spritesUnderMouse[0]->setSelected();

    if (InputManager::getInstance()->mousePressed(1) &&
            !spritesUnderMouse.empty()) {
        if (selectedVertex == NULL) {
            resetPath();
            selectedVertex = spritesUnderMouse[0];
        }
        else if (selectedVertex != spritesUnderMouse[0]) {
            path = Graph::getInstance()->findTheShortestPath(
                selectedVertex, spritesUnderMouse[0]
            );
            if (path.size() == 1) resetPath();
            resetSelectedVertex();
        }
        else {
            selectedVertex->setSelected();
            resetSelectedVertex();
        }
    }

    if (selectedVertex != NULL)
        selectedVertex->setSelected();
}

void ShortestPathMode::render()
{
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    if (path.size() > 1)
        Graph::getInstance()->drawPath(renderer, Game::EDGE_WIDTH, path);
}

void ShortestPathMode::quit()
{
    Logger::debug("ShortestPathMode::quit");
    resetPath();
}


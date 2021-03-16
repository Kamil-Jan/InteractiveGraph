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
    if (path.empty()) return;

    path[0]->resetSelected();
    Graph* graph = Graph::getInstance();
    for (size_t i=0; i < path.size() - 1; i++) {
        path[i+1]->resetSelected();
        graph->getEdge(path[i], path[i+1])->setLineTexture(
            Game::getInstance()->getEdgeTexture()
        );
    }
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

    if (path.size() <= 1) return;

    Graph* graph = Graph::getInstance();
    path[0]->setSelected();
    for (size_t i=0; i < path.size() - 1; i++) {
        Edge* edge = graph->getEdge(path[i], path[i + 1]);
        edge->setLineTexture(Game::getInstance()->getPathEdgeTexture());
        edge->drawEdge(renderer, Game::EDGE_WIDTH);
        path[i+1]->setSelected();
    }

    for (Vertex* vertex : path) vertex->render();
}

void ShortestPathMode::quit()
{
    Logger::debug("ShortestPathMode::quit");
    resetPath();
}


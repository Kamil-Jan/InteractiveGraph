#include <State/MainState.h>
#include <Mode/DeleteMode.h>
#include <InputManager.h>
#include <Graph/Graph.h>
#include <Logger.h>

void DeleteMode::quit()
{
    selectedVertex = NULL;
}

void DeleteMode::manageLeftMouseButtonClick(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse)
{
    Logger::debug("DeleteMode::manageLeftMouseButtonClick");
    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(1) &&
            !spritesUnderMouse.empty()) {
        Logger::debug("DeleteMode::update - remove vertex");
        graph->removeVertex(spritesUnderMouse[0]);
        if (selectedVertex != NULL && selectedVertex == spritesUnderMouse[0])
            selectedVertex = NULL;
    }
}

void DeleteMode::manageSelectedVertex(vector<Vertex*> spritesUnderMouse)
{
    Logger::debug("DeleteMode::manageSelectedVertex");
    if (!spritesUnderMouse.empty())
        spritesUnderMouse[0]->setSelected();

    if (selectedVertex != NULL)
        selectedVertex->setSelected();

    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(3) &&
            !spritesUnderMouse.empty()) {
        Vertex* vertexUnderMouse = spritesUnderMouse[0];
        if (selectedVertex == NULL) {
            Logger::debug("DeleteMode::manageSelectedVertex - select vertex");
            selectedVertex = vertexUnderMouse;
        }
        else if (selectedVertex != vertexUnderMouse) {
            if (graph->hasEdge(selectedVertex, vertexUnderMouse)) {
                Logger::debug("DeleteMode::manageSelectedVertex - remove edge");
                graph->removeEdge(selectedVertex, vertexUnderMouse);
            }
            selectedVertex = NULL;
        }
        else {
            selectedVertex = NULL;
        }
    }
}

void DeleteMode::update(float ticks)
{
    Logger::debug("DeleteMode::update");

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MainState* mainState = MainState::getInstance();
    vector<Vertex*> spritesUnderMouse = mainState->getSpritesUnderMouse(mouseX, mouseY);

    manageLeftMouseButtonClick(mouseX, mouseY, spritesUnderMouse);
    manageSelectedVertex(spritesUnderMouse);
}

void DeleteMode::render() {}


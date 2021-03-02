#include <Mode/DrawMode.h>
#include <State/MainState.h>
#include <InputManager.h>
#include <Logger.h>
#include <Game.h>

DrawMode::DrawMode() : Mode()
{
    Logger::debug("DrawMode::DrawMode");
}

void DrawMode::quit()
{
    selectedVertex = NULL;
    draggedVertex = NULL;
}

void DrawMode::manageLeftMouseButtonClick(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse)
{
    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(1)) {
        if (!MainState::getInstance()->getMainMenu()->isAround(mouseX, mouseY) &&
                !MainState::getInstance()->getMainMenu()->isInsideWidgets(mouseX, mouseY) &&
                spritesUnderMouse.empty()) {
            Logger::debug("DrawMode::manageLeftMouseButtonEvent - add vertex");
            graph->addVertex(
                new Vertex(
                    graph->getCounter() + 1,
                    mouseX, mouseY,
                    Game::VERTEX_RADIUS,
                    Game::VERTEX_BORDER)
            );
        }
    }
}

void DrawMode::manageSelectedVertex(vector<Vertex*> spritesUnderMouse)
{
    Logger::debug("DrawMode::manageSelectedVertex");

    if (!spritesUnderMouse.empty())
        spritesUnderMouse[0]->setSelected();

    if (selectedVertex != NULL)
        selectedVertex->setSelected();

    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(3) &&
            !spritesUnderMouse.empty()) {
        Vertex* vertexUnderMouse = spritesUnderMouse[0];
        if (selectedVertex == NULL) {
            Logger::debug("DrawMode::manageSelectedVertex - select vertex");
            selectedVertex = vertexUnderMouse;
        }
        else if (selectedVertex != vertexUnderMouse) {
            if (!graph->hasEdge(selectedVertex, vertexUnderMouse)) {
                Logger::debug("DrawMode::manageSelectedVertex - add edge");
                graph->addEdge(selectedVertex, vertexUnderMouse);
            }
            selectedVertex = NULL;
        }
        else {
            selectedVertex = NULL;
        }
    }
}

void DrawMode::manageDraggedVertex(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse)
{
    if (InputManager::getInstance()->isKeyDown(SDLK_LSHIFT) &&
            InputManager::getInstance()->isMouseDown(1) &&
            !spritesUnderMouse.empty() &&
            draggedVertex == NULL)
        draggedVertex = spritesUnderMouse[0];

    if (InputManager::getInstance()->mouseReleased(1))
        draggedVertex = NULL;

    if (draggedVertex != NULL &&
            !MainState::getInstance()->getMainMenu()->isAround(mouseX, mouseY) &&
            !MainState::getInstance()->getMainMenu()->isInsideWidgets(mouseX, mouseY)) {
        draggedVertex->setPosition(mouseX, mouseY);
    }
}

void DrawMode::update(float ticks)
{
    Logger::debug("DrawMode::update");

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MainState* mainState = MainState::getInstance();
    vector<Vertex*> spritesUnderMouse = mainState->getSpritesUnderMouse(mouseX, mouseY);

    manageLeftMouseButtonClick(mouseX, mouseY, spritesUnderMouse);
    manageDraggedVertex(mouseX, mouseY, spritesUnderMouse);
    manageSelectedVertex(spritesUnderMouse);
}

void DrawMode::render() {}


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
    curvingEdgeStart = NULL;
    curvingEdgeEnd = NULL;
    curving = false;
}

void DrawMode::manageLeftMouseButtonClick(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse)
{
    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(1) && !InputManager::getInstance()->isKeyDown(SDLK_LSHIFT)) {
        if (!MainState::getInstance()->isAroundMenus(mouseX, mouseY) &&
                !MainState::getInstance()->isInsideMenusWidgets(mouseX, mouseY) &&
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
            !InputManager::getInstance()->isKeyDown(SDLK_LSHIFT) &&
            !spritesUnderMouse.empty()) {
        Vertex* vertexUnderMouse = spritesUnderMouse[0];
        if (selectedVertex == NULL) {
            Logger::debug("DrawMode::manageSelectedVertex - select vertex");
            curving = false;
            curvingEdgeStart = NULL;
            selectedVertex = vertexUnderMouse;
        }
        else if (selectedVertex != vertexUnderMouse) {
            if (!graph->hasEdge(selectedVertex, vertexUnderMouse)) {
                Logger::debug("DrawMode::manageSelectedVertex - add edge");
                graph->addEdge(selectedVertex, vertexUnderMouse);
            }
            graph->getEdge(selectedVertex, vertexUnderMouse)->resetCurved();
            selectedVertex = NULL;
        }
        else {
            selectedVertex = NULL;
        }
    }
}

void DrawMode::manageCurvingEdge(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse)
{
    Logger::debug("DrawMode::manageCurvingEdge");

    if (curvingEdgeStart != NULL)
        curvingEdgeStart->setSelected();

    if (curvingEdgeEnd != NULL)
        curvingEdgeEnd->setSelected();

    if (InputManager::getInstance()->mousePressed(3) && curving) {
        curving = false;
        curvingEdgeStart = NULL;
        curvingEdgeEnd = NULL;
    }

    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(3) &&
            InputManager::getInstance()->isKeyDown(SDLK_LSHIFT) &&
            !spritesUnderMouse.empty()) {
        selectedVertex = NULL;
        Vertex* vertexUnderMouse = spritesUnderMouse[0];
        if (curvingEdgeStart == NULL) {
            Logger::debug("DrawMode::manageSelectedVertex - select vertex");
            curvingEdgeStart = vertexUnderMouse;
        }
        else if (graph->hasEdge(curvingEdgeStart, vertexUnderMouse) &&
                 curvingEdgeStart != vertexUnderMouse) {
            curvingEdgeEnd = vertexUnderMouse;
            curving = true;
            graph->getEdge(curvingEdgeStart, curvingEdgeEnd)->setCurved();
        }
        else {
            curvingEdgeStart = NULL;
        }
    }

    if (curving) {
        graph->getEdge(curvingEdgeStart, curvingEdgeEnd)->setCurvePoint(mouseX, mouseY);
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
            !MainState::getInstance()->isAroundMenus(mouseX, mouseY) &&
            !MainState::getInstance()->isInsideMenusWidgets(mouseX, mouseY)) {
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

    if (!curving) {
        manageLeftMouseButtonClick(mouseX, mouseY, spritesUnderMouse);
        manageDraggedVertex(mouseX, mouseY, spritesUnderMouse);
        manageSelectedVertex(spritesUnderMouse);
        manageCurvingEdge(mouseX, mouseY, spritesUnderMouse);
    } else {
        manageCurvingEdge(mouseX, mouseY, spritesUnderMouse);
    }
}

void DrawMode::render() {}


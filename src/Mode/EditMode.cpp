#include <iostream>
#include <State/MainState.h>
#include <Mode/EditMode.h>
#include <InputManager.h>
#include <Graph/Graph.h>
#include <Logger.h>
#include <Game.h>

EditMode::EditMode() : Mode()
{
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    editTex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        1, 1);

    SDL_SetRenderTarget(renderer, editTex);
    vector<int> editRectColor = Game::MENU_BORDER_COLOR;
    SDL_SetRenderDrawColor(renderer,
        editRectColor[0], editRectColor[1],
        editRectColor[2], editRectColor[3]
    );
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
}

EditMode::~EditMode()
{
    Logger::debug("EditMode::~EditMode");
    SDL_DestroyTexture(editTex);
}

void EditMode::manageLeftMouseButtonClick(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse)
{
    Logger::debug("EditMode::manageLeftMouseButtonClick");
    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(1) && edit) {
        edit = false;
        if (weightStr != "" && weightStr != "0") {
            graph->setEdgeWeight(
                editableEdge->getStart(),
                editableEdge->getEnd(),
                std::stoi(weightStr)
            );
        } else {
            editableEdge->setWeight(editableEdge->getWeight());
        }
    }
}

void EditMode::manageSelectedVertex(vector<Vertex*> spritesUnderMouse)
{
    Logger::debug("EditMode::manageSelectedVertex");
    if (!spritesUnderMouse.empty())
        spritesUnderMouse[0]->setSelected();

    if (selectedVertex != NULL)
        selectedVertex->setSelected();

    Graph* graph = Graph::getInstance();
    if (InputManager::getInstance()->mousePressed(3) &&
            !spritesUnderMouse.empty()) {
        Vertex* vertexUnderMouse = spritesUnderMouse[0];
        if (selectedVertex == NULL) {
            Logger::debug("EditMode::manageSelectedVertex - select vertex");
            selectedVertex = vertexUnderMouse;
        }
        else if (selectedVertex != vertexUnderMouse) {
            if (graph->hasEdge(selectedVertex, vertexUnderMouse)) {
                Logger::debug("EditMode::manageSelectedVertex - remove edge");
                edit = true;
                weightStr = "";
                editableEdge = graph->getEdge(selectedVertex, vertexUnderMouse);
            }
            selectedVertex = NULL;
        }
        else {
            selectedVertex = NULL;
        }
    }
}

void EditMode::update(float ticks)
{
    Logger::debug("EditMode::update");
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MainState* mainState = MainState::getInstance();
    vector<Vertex*> spritesUnderMouse = mainState->getSpritesUnderMouse(mouseX, mouseY);

    manageLeftMouseButtonClick(mouseX, mouseY, spritesUnderMouse);
    manageSelectedVertex(spritesUnderMouse);
    getUserInput();
}

void EditMode::getUserInput()
{
    if (!edit) return;

    vector<SDL_KeyCode> digitKeys = {
        SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
        SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9
    };

    bool weightChanged = false;
    InputManager* inputManager = InputManager::getInstance();
    std::string addedDigit;
    for (size_t i=0; i < digitKeys.size(); i++) {
        if (inputManager->keyReleased(digitKeys[i])) {
            addedDigit = std::to_string(i);
            weightChanged = true;
        }
    }

    if (weightStr != "" && inputManager->keyReleased(SDLK_BACKSPACE)) {
        weightStr.pop_back();
        weightChanged = true;
    }

    if (!weightChanged) return;

    if (weightStr.size() > 6) return;

    weightStr += addedDigit;

    int weight = (weightStr != "")? std::stoi(weightStr) : 0;

    SDL_Texture* weightTexture = Edge::createWeightTexture(weight);
    editableEdge->setWeightTexture(weightTexture);

    SDL_Rect prevWeightRect = editableEdge->getWeightRect();
    SDL_Rect weightRect = Edge::createWeightRect(weightTexture);

    int deltaWidth = weightRect.w - prevWeightRect.w;
    int deltaHeight = weightRect.h - prevWeightRect.h;
    weightRect.x = prevWeightRect.x - (deltaWidth)/2;
    weightRect.y = prevWeightRect.y - (deltaHeight)/2;

    editableEdge->setWeightRect(weightRect);
}

void EditMode::quit()
{
    edit = false;
    selectedVertex = NULL;
    weightStr = "";
}

void EditMode::render()
{
    if (!edit) return;

    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    int padding = 4;
    SDL_Rect weightRect = editableEdge->getWeightRect();
    SDL_Rect borderRect;
    borderRect.w = weightRect.w + padding;
    borderRect.h = weightRect.h + padding;
    borderRect.x = weightRect.x - padding/2;
    borderRect.y = weightRect.y - padding/2;

    int x1 = editableEdge->getStart()->getX();
    int y1 = editableEdge->getStart()->getY();
    int x2 = editableEdge->getEnd()->getX();
    int y2 = editableEdge->getEnd()->getY();
    int angle = 90 - 180 / 3.1415926 * atan2(x2 - x1, y2 - y1);
    if (angle > 90)
        angle += 180;

    SDL_RenderCopyEx(
        renderer, editTex,
        NULL, &borderRect,
        angle, NULL,
        SDL_FLIP_NONE
    );

    editableEdge->drawWeight(renderer);
}


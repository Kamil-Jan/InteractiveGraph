#include <iostream>
#include <iomanip>
#include <SDL2/SDL_ttf.h>

#include <Util.h>
#include <Game.h>
#include <Logger.h>
#include <InputManager.h>

Game* Game::instance = NULL;

const vector<int> Game::BG_COLOR = { 215, 215, 255, 255 };
const int Game::MENU_BORDER_WIDTH = 4;
const vector<int> Game::MENU_BG_COLOR = { 175, 175, 255, 255 };
const vector<int> Game::MENU_BORDER_COLOR = { 0, 0, 200, 255 };
const int Game::BUTTON_BORDER_WIDTH = 4;
const vector<int> Game::BUTTON_BG_COLOR = { 125, 125, 255, 255 };
const vector<int> Game::BUTTON_SELECTED_BG_COLOR = { 85, 85, 255, 255 };
const vector<int> Game::BUTTON_BORDER_COLOR = { 0, 0, 200, 255 };

const int Game::VERTEX_RADIUS = 25;
const int Game::VERTEX_BORDER = 3;
const vector<int> Game::VERTEX_COLOR = { 105, 90, 250, 255 };
const vector<int> Game::VERTEX_BORDER_COLOR = { 0, 0, 200, 255 };
const vector<int> Game::SELECTED_VERTEX_COLOR = { 175, 85, 250, 255 };
const vector<int> Game::PATH_VERTEX_COLOR = { 205, 85, 250, 255 };

const int Game::EDGE_WIDTH = 3;
const int Game::ARROW_LINE_LENGTH = 15;
const float Game::ARROW_LINE_ANGLE = 3.14 / 6;
const vector<int> Game::EDGE_COLOR = { 0, 0, 100, 255 };
const vector<int> Game::PATH_EDGE_COLOR = { 255, 0, 0, 255 };

Game::Game(string title)
{
    Logger::debug("Game::Game started");

    instance = instance? instance : this;
    storedState = NULL;

    // Initialize SDL library
    Logger::debug("Game::Game - initialize SDL");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(
            "Game::Game - cannot initialize SDL!\n" +
            (string) SDL_GetError()
        );

    // Enable anti-aliasing
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Initialize SDL_ttf library
    Logger::debug("Game::Game - initialize SDL_ttf");
    if (TTF_Init() != 0)
        throw std::runtime_error(
            "Game::Game - cannot initialize SDL_ttf!\n" +
            (string) SDL_GetError()
        );

    // Load font
    font = TTF_OpenFont("res/LiberationMono.ttf", 22);
    if (font == NULL)
        throw std::runtime_error("Vertex::Vertex - invalid font path");


    // Create main window
    Logger::debug("Game::Game - initialize window");
    window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            0);
    if (window == NULL)
        throw std::runtime_error(
            "Game::Game - cannot initialize window!\n" +
            (string) SDL_GetError()
        );

    // Initialize Renderer
    Logger::debug("Game::Game - initialize renderer");
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    if (renderer == NULL)
        throw std::runtime_error(
            "Game::Game - cannot initialize renderer!\n" +
            (string) SDL_GetError()
        );

    // Create window surface
    Logger::debug("Game::Game - initialize screen");
    screen = SDL_GetWindowSurface(window);
    if (screen == NULL)
        throw std::runtime_error(
            "Game::Game - cannot initialize screen!\n" +
            (string) SDL_GetError()
        );

    vertexTexture = Util::createBorderedCircleTexture(
        renderer, VERTEX_RADIUS, VERTEX_BORDER,
        VERTEX_COLOR, VERTEX_BORDER_COLOR
    );

    selectedVertexTexture = Util::createBorderedCircleTexture(
        renderer, VERTEX_RADIUS, VERTEX_BORDER,
        SELECTED_VERTEX_COLOR, VERTEX_BORDER_COLOR
    );

    edgeTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetRenderDrawColor(renderer, EDGE_COLOR[0], EDGE_COLOR[1],
                           EDGE_COLOR[2], EDGE_COLOR[3]);
    SDL_SetRenderTarget(renderer, edgeTexture);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    pathEdgeTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetRenderDrawColor(renderer, PATH_EDGE_COLOR[0], PATH_EDGE_COLOR[1],
                           PATH_EDGE_COLOR[2], PATH_EDGE_COLOR[3]);
    SDL_SetRenderTarget(renderer, pathEdgeTexture);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    Logger::debug("Game::Game ended");
}

Game::~Game()
{
    Logger::debug("Game::~Game");
    SDL_DestroyTexture(vertexTexture);
    SDL_DestroyTexture(selectedVertexTexture);
    SDL_DestroyTexture(edgeTexture);
    SDL_DestroyTexture(pathEdgeTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

State* Game::getCurrentState()
{
    Logger::debug("Game::getCurrentState");
    if (!stateStack.empty())
        return stateStack.top();
    else
        throw std::runtime_error("Game::getCurrentState - stateStack is empty!");
}

void Game::push(State* state)
{
    Logger::debug("Game::push");
    storedState = state;
}

void Game::manageStack()
{
    Logger::debug("Game::manageStack started");

    if (storedState != NULL) {
        Logger::debug("Game::manageStack - storedState != NULL");
        Logger::debug("Game::manageStack - push to the stateStack");
        stateStack.emplace(storedState);
        storedState = NULL;
    }

    if (getCurrentState()->quitRequested()) {
        Logger::debug("Game::manageStack - pop from the stateStack");
        stateStack.pop();
    }

    Logger::debug("Game::manageStack ended");
}

void Game::run()
{
    Logger::debug("Game::run started");
    manageStack();

    Uint32 startingTicks, frameTicks;
    SDL_Rect fpsRect;
    fpsRect.x = SCREEN_WIDTH - 90;
    fpsRect.y = SCREEN_HEIGHT - 50;
    fpsRect.w = 80;
    fpsRect.h = 40;
    string fps;
    while (!stateStack.empty()) {
        Uint64 start = SDL_GetPerformanceCounter();
        startingTicks = SDL_GetTicks();

        InputManager::getInstance()->update();

        if (InputManager::getInstance()->quitRequested()) {
            while (!stateStack.empty()) stateStack.pop();
            break;
        }

        Logger::debug("Game::run - update current state");
        getCurrentState()->update();

        Logger::debug("Game::run - render current state");
        getCurrentState()->render();

        SDL_Surface* fpsSurface = TTF_RenderText_Solid(font, fps.c_str(), { 0, 0, 0, 255 });
        SDL_Texture* fpsTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);
        SDL_RenderCopy(renderer, fpsTexture, NULL, &fpsRect);

        Logger::debug("Game::run - render present");
        SDL_RenderPresent(renderer);

        manageStack();
        frameTicks = SDL_GetTicks() - startingTicks;
        capFrameRate(frameTicks);

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsed = ( end - start ) / (float) SDL_GetPerformanceFrequency();
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << 1.0f / elapsed;
        fps = stream.str();

        SDL_FreeSurface(fpsSurface);
        SDL_DestroyTexture(fpsTexture);
    }
    Logger::debug("Game::run ended");
}

void Game::capFrameRate(float delta)
{
    Logger::debug("Game::capFrameRate");
    if (TICKS_PER_FRAME > delta)
        SDL_Delay(TICKS_PER_FRAME - delta);
}

Game* Game::getInstance()
{
    return instance;
}

SDL_Renderer* Game::getRenderer()
{
    return renderer;
}

SDL_PixelFormat* Game::getScreenFormat()
{
    return screen->format;
}

TTF_Font* Game::getFont()
{
    return font;
}

SDL_Texture* Game::getVertexTexture()
{
    return vertexTexture;
}

SDL_Texture* Game::getSelectedVertexTexture()
{
    return selectedVertexTexture;
}

SDL_Texture* Game::getEdgeTexture()
{
    return edgeTexture;
}

SDL_Texture* Game::getPathEdgeTexture()
{
    return pathEdgeTexture;
}


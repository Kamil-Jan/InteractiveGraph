#include <SDL2/SDL.h>
#include <State/HelpState.h>
#include <Action/BackAction.h>
#include <InputManager.h>
#include <Logger.h>
#include <Game.h>

HelpState::HelpState() : State()
{
    Logger::debug("HelpState::HelpState");
    shortcutsImg = IMG_LoadTexture(Game::getInstance()->getRenderer(), "res/shortcuts.jpg");

    int w, h;
    SDL_QueryTexture(shortcutsImg, NULL, NULL, &w, &h);
    shortcutsRect.x = SCREEN_WIDTH/2 - w/2 + 30;
    shortcutsRect.y = 0.15*SCREEN_HEIGHT;
    shortcutsRect.w = w;
    shortcutsRect.h = h;

    backButton = new Button(
        "Back",
        new BackAction<State>(this),
        SCREEN_WIDTH - 110, 10, 100, 50,
        Game::BUTTON_BORDER_WIDTH,
        Game::BUTTON_BG_COLOR,
        Game::BUTTON_SELECTED_BG_COLOR,
        Game::BUTTON_BORDER_COLOR
    );
}

HelpState::~HelpState()
{
    Logger::debug("HelpState::~HelpState");
    SDL_DestroyTexture(shortcutsImg);
}

void HelpState::update()
{
    Logger::debug("HelpState::update");
    backButton->update(SDL_GetTicks());
}

void HelpState::render()
{
    Logger::debug("HelpState::HelpState");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();

    vector<int> bgColor = Game::BG_COLOR;
    SDL_SetRenderDrawColor(renderer, bgColor[0],
                           bgColor[1], bgColor[2],
                           bgColor[3]);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, shortcutsImg, NULL, &shortcutsRect);
    backButton->render();
}


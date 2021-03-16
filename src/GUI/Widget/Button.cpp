#include <SDL2/SDL_ttf.h>
#include <Util.h>
#include <Game.h>
#include <Logger.h>
#include <InputManager.h>
#include <GUI/Widget/Button.h>

Button::Button(string text, Action* action,
               int x, int y, int w, int h,
               int borderWidth, vector<int> bgColor,
               vector<int> selectedBgColor,
               vector<int> borderColor) :
    Widget(x, y, w, h)
{
    Logger::debug("Button::Button");
    this->text = text;
    this->action = action;
    this->bgColor = bgColor;
    this->curColor = bgColor;
    this->selectedBgColor = selectedBgColor;
    this->borderWidth = borderWidth;
    this->borderColor = borderColor;

    if (!text.empty()) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            Game::getInstance()->getFont(),
            text.c_str(), { 255, 255, 255, 255 }
        );
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = getX() + (getWidth() - textRect.w)/2;
        textRect.y = getY() + (getHeight() - textRect.h)/2;

        textTexture = SDL_CreateTextureFromSurface(
            Game::getInstance()->getRenderer(),
            textSurface
        );
        SDL_FreeSurface(textSurface);
    }
}

Button::~Button()
{
    Logger::debug("Button::~Button");
    SDL_DestroyTexture(textTexture);
}

string Button::getText()
{
    return text;
}

void Button::reset()
{
    Logger::debug("Button::reset");
    selected = false;
}

bool Button::getSelected()
{
    Logger::debug("Button::getSelected");
    return selected;
}

void Button::setSelected()
{
    Logger::debug("Button::setSelected");
    selected = true;
}

bool Button::isInside(int x, int y)
{
    Logger::debug("Button::isInside");
    SDL_Point point = { x, y };
    return SDL_PointInRect(&point, &rect);
}

void Button::update(float ticks)
{
    Logger::debug("Button::update");
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    selected = false;
    if (!isInside(mouseX, mouseY)) {
        curColor = bgColor;
        return;
    }

    selected = true;
    curColor = selectedBgColor;

    if (InputManager::getInstance()->mousePressed(1) &&
            action != NULL) {
        action->execute();
    }
}

void Button::render()
{
    Logger::debug("Button::render");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    Util::drawRectWithBorder(renderer, rect, borderWidth,
                             curColor, borderColor);

    Util::drawText(renderer, textTexture, textRect);
}


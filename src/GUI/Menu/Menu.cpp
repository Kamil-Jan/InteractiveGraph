#include <SDL2/SDL.h>
#include <iostream>

#include <GUI/Menu/Menu.h>
#include <Logger.h>
#include <Util.h>
#include <Game.h>

Menu::Menu(int x, int y, int width, int height,
           int borderWidth, vector<int> bgColor,
           vector<int> borderColor) :
    Sprite(x, y, width, height)
{
    Logger::debug("Menu::Menu");
    if (bgColor.size() != 4 || borderColor.size() != 4) {
        throw std::runtime_error("Menu::Menu - invalid color");
    }

    this->borderWidth = borderWidth;
    this->bgColor = bgColor;
    this->borderColor = borderColor;
}

void Menu::addWidget(Widget* widget)
{
    widgets.add(widget);
}

bool Menu::isInside(int x, int y)
{
    Logger::debug("Mode::isInside");
    SDL_Point point = { x, y };
    return SDL_PointInRect(&point, &rect);
}

bool Menu::isAround(int x, int y)
{
    Logger::debug("Mode::isAround");
    SDL_Rect menuRect;
    menuRect.x = getX();
    menuRect.y = getY();
    menuRect.w = getWidth() + 20;
    menuRect.h = getHeight() + 20;

    SDL_Point point = { x, y };
    return SDL_PointInRect(&point, &menuRect);
}

bool Menu::isInsideWidgets(int x, int y)
{
    for (Widget* widget : widgets.getSprites()) {
        if (widget->isInside(x, y)) return true;
    }
    return false;
}

void Menu::resetWidgets()
{
    for (Widget* widget : widgets.getSprites())
        widget->reset();
}

void Menu::update(float ticks)
{
    Logger::debug("Menu::update");
    widgets.update(ticks);
}

void Menu::render()
{
    Logger::debug("Menu::render");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    Util::drawRectWithBorder(renderer, rect, borderWidth,
                             bgColor, borderColor);
    widgets.render();
}


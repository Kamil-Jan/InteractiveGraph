#include <iostream>
#include <Game.h>
#include <Util.h>
#include <Logger.h>
#include <GUI/Widget/Label.h>

Label::Label(std::string text, TTF_Font* font,
             std::vector<int> fgColor,
             int x, int y, int w, int h) :
    Widget(x, y, w, h)
{
    Logger::debug("Label::Label");
    this->text = text;
    this->font = font;
    this->fgColor = fgColor;
    borderWidth = 0;
    bgColor = {};
    borderColor = {};
    generateTextTexture();
}

Label::Label(std::string text, TTF_Font* font,
             std::vector<int> fgColor,
             int x, int y, int w, int h,
             int borderWidth, std::vector<int> bgColor,
             std::vector<int> borderColor) :
    Widget(x, y, w, h)
{
    Logger::debug("Label::Label");
    this->text = text;
    this->font = font;
    this->fgColor = fgColor;
    this->borderWidth = borderWidth;
    this->bgColor = bgColor;
    this->borderColor = borderColor;
    generateTextTexture();
}

Label::~Label()
{
    Logger::debug("Label::~Label");
    SDL_DestroyTexture(textTexture);
}

void Label::generateTextTexture()
{
    Logger::debug("Label::generateTextTexture");
    if (text.empty()) return;

    if (fgColor.size() != 4) {
        throw std::runtime_error("Invalid fg color");
    }

    SDL_Color fontColor = {
        (Uint8) fgColor[0],
        (Uint8) fgColor[1],
        (Uint8) fgColor[2],
        (Uint8) fgColor[3],
    };

    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(
        font, text.c_str(), fontColor, getWidth()
    );

    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    if (textRect.h > getHeight()) rect.h = textRect.h;
    textRect.x = getX() + (getWidth() - textRect.w)/2;
    textRect.y = getY() + (getHeight() - textRect.h)/2;

    textTexture = SDL_CreateTextureFromSurface(
        Game::getInstance()->getRenderer(),
        textSurface
    );
    SDL_FreeSurface(textSurface);
}

void Label::render()
{
    Logger::debug("Label::render");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    if (borderWidth != 0)
        Util::drawRectWithBorder(renderer, rect, borderWidth,
                                 bgColor, borderColor);

    Util::drawText(renderer, textTexture, textRect);
}

bool Label::isInside(int x, int y)
{
    return false;
}

void Label::update(float ticks) {}
void Label::reset() {}


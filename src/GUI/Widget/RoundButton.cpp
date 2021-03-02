#include <Game.h>
#include <Util.h>
#include <GUI/Widget/RoundButton.h>

RoundButton::RoundButton(SDL_Texture* innerContentTex,
                         Action* action, int x, int y,
                         int radius, int borderWidth,
                         vector<int> bgColor,
                         vector<int> selectedBgColor,
                         vector<int> borderColor)  :
    Button("", action, x, y, 2*radius, 2*radius, borderWidth,
           bgColor, selectedBgColor, borderColor)
{
    this->radius = radius;
    this->innerContentTex = innerContentTex;

    circleTex = Util::createBorderedCircleTexture(
        Game::getInstance()->getRenderer(),
        radius, borderWidth,
        bgColor, borderColor
    );

    selectedCircleTex = Util::createBorderedCircleTexture(
        Game::getInstance()->getRenderer(),
        radius, borderWidth,
        selectedBgColor, borderColor
    );
}

RoundButton::~RoundButton()
{
    SDL_DestroyTexture(circleTex);
    SDL_DestroyTexture(selectedCircleTex);
    SDL_DestroyTexture(innerContentTex);
}

bool RoundButton::isInside(int x, int y)
{
    return (Util::calculateDistance(getX(), getY(), x, y) <= radius);
}

void RoundButton::render()
{
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    SDL_Rect buttonRect;
    buttonRect.x = getX() - radius;
    buttonRect.y = getY() - radius;
    buttonRect.w = 2 * radius;
    buttonRect.h = 2 * radius;

    SDL_Rect innerContentRect;
    innerContentRect.x = getX() - radius + 2*borderWidth;
    innerContentRect.y = getY() - radius + 2*borderWidth;
    innerContentRect.w = 2 * radius - 4*borderWidth;
    innerContentRect.h = 2 * radius - 4*borderWidth;

    if (selected == true)
        SDL_RenderCopy(renderer, selectedCircleTex, NULL, &buttonRect);
    else
        SDL_RenderCopy(renderer, circleTex, NULL, &buttonRect);

    SDL_RenderCopy(renderer, innerContentTex, NULL, &innerContentRect);
}


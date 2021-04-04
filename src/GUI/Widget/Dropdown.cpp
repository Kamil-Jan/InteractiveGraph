#include <iostream>
#include <GUI/Widget/Dropdown.h>
#include <InputManager.h>
#include <Util.h>
#include <Game.h>
#include <Logger.h>

Dropdown::Dropdown(unordered_map<string, Action*> items,
                   string defaultText,
                   int x, int y,
                   int w, int h, int borderWidth,
                   vector<int> bgColor,
                   vector<int> textBgColor,
                   vector<int> selectedTextBgColor,
                   vector<int> borderColor) :
    Widget(x, y, w, h)
{
    Logger::debug("Dropdown::Dropdown");

    if (items.size() == 0)
        throw std::runtime_error("Dropdown::Dropdown - invalid items");

    this->items = items;
    this->itemsSize = items.size();
    this->borderWidth = borderWidth;
    this->bgColor = bgColor;
    this->textBgColor = textBgColor;
    this->selectedTextBgColor = selectedTextBgColor;
    this->borderColor = borderColor;

    this->defaultText = defaultText;

    SDL_Surface* defaultTextSurface = TTF_RenderText_Blended(
        Game::getInstance()->getFont(),
        defaultText.c_str(),
        { 255, 255, 255 }
    );

    defaultTextTexture = SDL_CreateTextureFromSurface(
        Game::getInstance()->getRenderer(),
        defaultTextSurface
    );

    defaultTextRect.w = defaultTextSurface->w;
    defaultTextRect.h = defaultTextSurface->h;
    SDL_FreeSurface(defaultTextSurface);

    int itemHeight = getHeight() - 2 * padding;
    int itemsListHeight = itemsSize * itemHeight + (itemsSize + 1) * padding - borderWidth;

    itemsListHeight = (itemsSize > shownItemNum)? itemsListHeight : itemsListHeight - borderWidth;

    itemsListTexture = SDL_CreateTexture(
        Game::getInstance()->getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        getWidth() - 2*borderWidth,
        itemsListHeight
    );
    processItems(items);

    int itemsNum = (itemsSize >= shownItemNum)? shownItemNum : itemsSize;
    int expandedHeight = itemsNum * itemHeight + (itemsNum-1) * padding + 2*borderWidth;
    itemsListRect.x = getX();
    itemsListRect.y = getY() + getHeight() - borderWidth/2;
    itemsListRect.w = getWidth();
    itemsListRect.h = expandedHeight;

    itemsListSrcRect.x = 0;
    itemsListSrcRect.y = 0;
    itemsListSrcRect.w = getWidth();
    itemsListSrcRect.h = expandedHeight - borderWidth;

    scrollBarRect.x = getX() + getWidth() - 50 + borderWidth + padding;
    scrollBarRect.y = getY() + getHeight() - borderWidth/2 + padding;
    scrollBarRect.w = 50 - borderWidth - 2 * padding;
    scrollBarRect.h = expandedHeight - 2 * padding;

    sliderRect.x = scrollBarRect.x + padding;
    sliderRect.y = scrollBarRect.y + padding;
    sliderRect.w = scrollBarRect.w - 2 * padding;
    if (itemsSize > shownItemNum)
        sliderRect.h = (scrollBarRect.h - padding) / (itemsSize - shownItemNum + 1);
    else
        sliderRect.h = scrollBarRect.h - 2 * padding;
}

Dropdown::~Dropdown()
{
    Logger::debug("Dropdown::~Dropdown");
    for (auto tex : textureMap)
        SDL_DestroyTexture(tex.second);
}

bool Dropdown::isInside(int x, int y)
{
    Logger::debug("Dropdown::isInside");
    SDL_Point point = { x, y };

    if (!expanded)
        return SDL_PointInRect(&point, &rect);

    return SDL_PointInRect(&point, &rect) || isInsideExpanded(x, y);
}

bool Dropdown::isInsideExpanded(int x, int y)
{
    SDL_Point point = { x, y };
    return SDL_PointInRect(&point, &itemsListRect);
}

bool Dropdown::isExpanded()
{
    return expanded;
}

string Dropdown::getSelectedItem()
{
    Logger::debug("Dropdown::getSelectedItem");
    return selected;
}

void Dropdown::reset()
{
    Logger::debug("Dropdown::reset");
    selected = "";
    expanded = false;
    sliderRect.y = scrollBarRect.y + padding;
    itemsListSrcRect.y = 0;
}

void Dropdown::processItems(unordered_map<string, Action*> items)
{
    Logger::debug("Dropdown::processItems");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    SDL_SetRenderTarget(renderer, itemsListTexture);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(itemsListTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);

    SDL_Texture* tex;
    SDL_Surface* textSurface;

    SDL_Rect itemRect;
    itemRect.x = padding;
    itemRect.y = 0;
    itemRect.w = getWidth() - 50;
    itemRect.h = getHeight() - 2*padding;

    for (auto item : items) {
        string itemStr = item.first;
        textSurface = TTF_RenderText_Blended(
            Game::getInstance()->getFont(),
            itemStr.c_str(), { 255, 255, 255 }
        );

        tex = SDL_CreateTextureFromSurface(
            Game::getInstance()->getRenderer(),
            textSurface
        );

        textureMap[itemStr] = tex;

        SDL_Rect textRect;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = itemRect.x + (itemRect.w - textRect.w)/2;
        textRect.y = itemRect.y + (itemRect.h - textRect.h)/2;

        textRectMap[itemStr] = textRect;

        Util::drawRectWithBorder(renderer, itemRect,
                                 borderWidth, textBgColor,
                                 borderColor);
        Util::drawText(renderer, tex, textRect);

        itemsRectMap[itemStr] = itemRect;
        itemRect.y += itemRect.h + padding;
    }

    SDL_FreeSurface(textSurface);
    SDL_SetRenderTarget(renderer, NULL);
}

void Dropdown::update(float ticks)
{
    Logger::debug("Dropdown::update");
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (!expanded && InputManager::getInstance()->mousePressed(1) &&
            isInside(mouseX, mouseY)) {
        expanded = true;
        return;
    }

    if (!expanded)
        return;

    SDL_Point point = { mouseX, mouseY };
    SDL_Rect itemRect;
    if (InputManager::getInstance()->mousePressed(1) && isInsideExpanded(mouseX, mouseY)) {
        for (auto item : items) {
            string itemStr = item.first;
            itemRect.x = itemsRectMap[itemStr].x;
            itemRect.y = itemsListRect.y + itemsRectMap[itemStr].y - itemsListSrcRect.y;
            itemRect.w = itemsRectMap[itemStr].w;
            itemRect.h = itemsRectMap[itemStr].h;

            if (SDL_PointInRect(&point, &itemRect)) {
                items[itemStr]->execute();
                selected = itemStr;
                expanded = false;
                break;
            }
        }
    }

    if (InputManager::getInstance()->mousePressed(1) && !isInsideExpanded(mouseX, mouseY))
        expanded = false;

    if (InputManager::getInstance()->isMouseDown(1) &&
            isInsideExpanded(mouseX, mouseY) &&
            SDL_PointInRect(&point, &sliderRect)) {
        sliderDragged = true;
    }

    int sliderY0 = scrollBarRect.y + padding;
    int delta = mouseY - sliderY0;
    int maxLevel = itemsSize - shownItemNum;
    int level = delta / sliderRect.h;

    if (sliderDragged) {
        level = (level > maxLevel)? maxLevel : level;
        if (level < 0) {
            sliderRect.y = sliderY0;
            itemsListSrcRect.y = 0;
        } else if (level == maxLevel) {
            sliderRect.y = scrollBarRect.y + scrollBarRect.h - sliderRect.h - padding;
            itemsListSrcRect.y = maxLevel * (getHeight() - padding);
        } else {
            sliderRect.y = sliderY0 + level * sliderRect.h;
            itemsListSrcRect.y = level * (getHeight() - padding);
        }
    }

    if (sliderDragged && InputManager::getInstance()->mouseReleased(1)) {
        sliderDragged = false;
    }
}

void Dropdown::renderItems(SDL_Renderer* renderer)
{
    Logger::debug("Dropdown::renderItems");

    Util::drawRectWithBorder(renderer, itemsListRect, borderWidth, bgColor, borderColor);

    SDL_Rect itemsRect;
    itemsRect.x = itemsListRect.x;
    itemsRect.y = itemsListRect.y + padding;
    itemsRect.w = itemsListRect.w - 2*borderWidth;
    itemsRect.h = (itemsSize > shownItemNum)? itemsListRect.h - borderWidth :
        itemsListRect.h - 2*borderWidth;

    SDL_RenderCopy(renderer, itemsListTexture, &itemsListSrcRect, &itemsRect);
}

void Dropdown::drawArrowDown(SDL_Renderer *renderer)
{
    Logger::debug("Dropdown::drawArrowDown");
    int centerX = getX() + getWidth() - 25;
    int centerY = getY() + getHeight() / 2;

    int dX = 8;
    int dY = 5;

    int x1 = centerX - dX;
    int y1 = centerY - dY;

    int x2 = centerX + dX;
    int y2 = centerY - dY;

    int x3 = centerX;
    int y3 = centerY + dY;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    Util::drawLine(renderer, Game::getInstance()->getEdgeTexture(),
                   x1, y1, x3, y3, 0.7*borderWidth);
    Util::drawLine(renderer, Game::getInstance()->getEdgeTexture(),
                   x2, y2, x3, y3, 0.7*borderWidth);
}

void Dropdown::drawArrowUp(SDL_Renderer *renderer)
{
    Logger::debug("Dropdown::drawArrowUp");
    int centerX = getX() + getWidth() - 25;
    int centerY = getY() + getHeight() / 2;

    int dX = 8;
    int dY = 5;

    int x1 = centerX - dX;
    int y1 = centerY + dY;

    int x2 = centerX + dX;
    int y2 = centerY + dY;

    int x3 = centerX;
    int y3 = centerY - dY;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    Util::drawLine(renderer, Game::getInstance()->getEdgeTexture(),
                   x1, y1, x3, y3, 0.7*borderWidth);
    Util::drawLine(renderer, Game::getInstance()->getEdgeTexture(),
                   x2, y2, x3, y3, 0.7*borderWidth);
}

void Dropdown::render()
{
    Logger::debug("Dropdown::render");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    Util::drawRectWithBorder(renderer, rect, borderWidth, bgColor, borderColor);

    SDL_Rect itemRect;
    itemRect.x = getX() + padding;
    itemRect.y = getY() + padding;
    itemRect.w = getWidth() - 50;
    itemRect.h = getHeight() - 2*padding;

    Util::drawRectWithBorder(renderer, itemRect, borderWidth, textBgColor, borderColor);
    if (selected != "") {
        SDL_Rect selectedTextRect = textRectMap[selected];
        selectedTextRect.x += 2*borderWidth;
        selectedTextRect.y = getY() + (getHeight() - selectedTextRect.h)/2;

        Util::drawText(renderer, textureMap[selected], selectedTextRect);
    } else {
        defaultTextRect.x = getX() + padding + (getWidth() - 50 - defaultTextRect.w)/2;
        defaultTextRect.y = getY() + (getHeight() - defaultTextRect.h)/2;
        Util::drawText(renderer, defaultTextTexture, defaultTextRect);
    }

    if (expanded) {
        renderItems(renderer);
        Util::drawRectWithBorder(renderer, scrollBarRect, borderWidth, textBgColor, borderColor);
        Util::drawRectWithBorder(renderer, sliderRect, borderWidth, selectedTextBgColor, borderColor);
        drawArrowUp(renderer);
    } else {
        drawArrowDown(renderer);
    }
}


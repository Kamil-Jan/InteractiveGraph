#include <iostream>

#include <Graph/Edge.h>
#include <Logger.h>
#include <Game.h>
#include <Util.h>

Edge::Edge(Vertex* start, Vertex* end,
           int weight, bool directed,
           SDL_Texture* lineTexture)
{
    Logger::debug("Edge::Edge");
    this->start = start;
    this->end = end;
    this->directed = directed;
    this->lineTexture = lineTexture;
    setWeight(weight);
}

Vertex* Edge::getStart()
{
    return start;
}

Vertex* Edge::getEnd()
{
    return end;
}

int Edge::getWeight()
{
    return weight;
}

void Edge::setCurved()
{
    curved = true;
}

void Edge::resetCurved()
{
    curved = false;
    curvePointX = -1;
    curvePointY = -1;
}

SDL_Texture* Edge::getWeightTexture()
{
    return weightTexture;
}

SDL_Rect Edge::getWeightRect()
{
    return weightRect;
}

void Edge::setWeight(int weight)
{
    this->weight = weight;
    weightTexture = createWeightTexture(weight);
    weightRect = createWeightRect(weightTexture);
}

void Edge::setWeightTexture(SDL_Texture* texture)
{
    weightTexture = texture;
}

void Edge::setLineTexture(SDL_Texture* texture)
{
    lineTexture = texture;
}

void Edge::setWeightRect(SDL_Rect rect)
{
    weightRect = rect;
}

bool Edge::isDirected()
{
    return directed;
}

void Edge::setCurvePoint(int x, int y)
{
    curvePointX = x;
    curvePointY = y;
}

SDL_Texture* Edge::createWeightTexture(int weight)
{
    Logger::debug("Edge::createWeightTexture");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    vector<int> textColorVector = Game::EDGE_COLOR;
    SDL_Color txtColor = {
        (Uint8) textColorVector[0],
        (Uint8) textColorVector[1],
        (Uint8) textColorVector[2]
    };

    string weigthStr = (weight > 0)? std::to_string(weight) : " ";
    SDL_Surface* weightSurface = TTF_RenderText_Solid(
        Game::getInstance()->getFont(),
        weigthStr.c_str(),
        txtColor
    );

    int padding = 6;
    SDL_Texture* weightTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        weightSurface->w + padding,
        weightSurface->h + padding
    );

    SDL_SetRenderTarget(renderer, weightTexture);
    vector<int> bgColorVector = Game::BG_COLOR;
    SDL_SetRenderDrawColor(
        renderer,
        bgColorVector[0],
        bgColorVector[1],
        bgColorVector[2],
        bgColorVector[3]
    );
    SDL_RenderClear(renderer);

    SDL_Rect textRect;
    textRect.w = weightSurface->w;
    textRect.h = weightSurface->h;
    textRect.x = padding/2;
    textRect.y = padding/2;

    SDL_RenderCopy(
        renderer,
        SDL_CreateTextureFromSurface(renderer, weightSurface),
        NULL,
        &textRect
    );

    SDL_SetRenderTarget(renderer, NULL);
    SDL_FreeSurface(weightSurface);
    return weightTexture;
}

SDL_Rect Edge::createWeightRect(SDL_Texture *texture)
{
    Logger::debug("Edge::createWeightRect");
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect weightRect;
    weightRect.w = width;
    weightRect.h = height;
    return weightRect;
}

void Edge::drawWeight(SDL_Renderer* renderer)
{
    Logger::debug("Edge::drawWeight");
    int x1 = start->getX();
    int y1 = start->getY();
    int x2 = end->getX();
    int y2 = end->getY();

    int angle = 90 - 180 / 3.1415926 * atan2(x2 - x1, y2 - y1);
    if (angle > 90)
        angle += 180;

    if (curved) {
        vector<int> vx = { x1, curvePointX, x2 };
        vector<int> vy = { y1, curvePointY, y2 };
        SDL_FPoint lineCenter = Util::getCurvedLinePoint(vx, vy, 0.5);
        x1 = lineCenter.x - 10;
        y1 = lineCenter.y - 10;
        x2 = lineCenter.x + 10;
        y2 = lineCenter.y + 10;
    }

    weightRect.x = x1 + (x2 - x1)/2 - weightRect.w/2;
    weightRect.y = y1 + (y2 - y1)/2 - weightRect.h/2;

    SDL_RenderCopyEx(
        renderer, weightTexture,
        NULL, &weightRect,
        angle, NULL, SDL_FLIP_NONE
    );
}

void Edge::drawArrows(SDL_Renderer* renderer, int lineWidth)
{
    Logger::debug("Edge::drawArrows");
    double x1 = start->getX();
    double y1 = start->getY();
    double x2 = end->getX();
    double y2 = end->getY();

    if (curved) {
        vector<int> vx = { (int) x1, curvePointX, (int) x2 };
        vector<int> vy = { (int) y1, curvePointY, (int) y2 };
        double dis = 0;
        double t = 1.0;
        double tDelta = 0.01;
        while (dis < (double) Game::VERTEX_RADIUS) {
            SDL_FPoint lineTail = Util::getCurvedLinePoint(vx, vy, t);
            x1 = lineTail.x;
            y1 = lineTail.y;
            dis = Util::calculateDistanceF(x1, y1, x2, y2);
            t -= tDelta;
        }
    }

    double distance = Util::calculateDistanceF(x1, y1, x2, y2);
    double deltaX = (Game::VERTEX_RADIUS * (x1 - x2)) / (distance);
    double deltaY = (Game::VERTEX_RADIUS * (y1 - y2)) / (distance);

    double interceptX = x2 + deltaX;
    double interceptY = y2 + deltaY;

    double edgeAngle = atan((double) (y2 - y1) / (x2 - x1));
    double alpha = edgeAngle + Game::ARROW_LINE_ANGLE;
    double betha = edgeAngle - Game::ARROW_LINE_ANGLE;

    double line1X, line1Y, line2X, line2Y;
    if (x2 < x1) {
        line1X = interceptX + Game::ARROW_LINE_LENGTH * cos(alpha);
        line1Y = interceptY + Game::ARROW_LINE_LENGTH * sin(alpha);
        line2X = interceptX + Game::ARROW_LINE_LENGTH * cos(betha);
        line2Y = interceptY + Game::ARROW_LINE_LENGTH * sin(betha);
    } else {
        line1X = interceptX - Game::ARROW_LINE_LENGTH * cos(alpha);
        line1Y = interceptY - Game::ARROW_LINE_LENGTH * sin(alpha);
        line2X = interceptX - Game::ARROW_LINE_LENGTH * cos(betha);
        line2Y = interceptY - Game::ARROW_LINE_LENGTH * sin(betha);
    }

    Util::drawLineF(renderer, lineTexture,
                   interceptX, interceptY,
                   line1X, line1Y,
                   lineWidth);
    Util::drawLineF(renderer, lineTexture,
                   interceptX, interceptY,
                   line2X, line2Y,
                   lineWidth);
}

void Edge::drawEdge(SDL_Renderer* renderer, int lineWidth)
{
    Logger::debug("Edge::drawEdge");

    int x1 = start->getX();
    int y1 = start->getY();
    int x2 = end->getX();
    int y2 = end->getY();

    if (!curved) {
        Util::drawLineF(renderer, lineTexture, x1, y1, x2, y2, lineWidth);
        drawWeight(renderer);
    } else {
        vector<int> vx = { x1, curvePointX, x2 };
        vector<int> vy = { y1, curvePointY, y2 };
        Util::drawCurvedLine(renderer, lineTexture, vx, vy, 30, lineWidth);

        drawWeight(renderer);
    }

    if (directed) {
        drawArrows(renderer, lineWidth);
    }
}

void Edge::render()
{
    drawEdge(Game::getInstance()->getRenderer(), Game::EDGE_WIDTH);
}


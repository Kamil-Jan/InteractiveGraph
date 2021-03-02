#include <iostream>

#include <Util.h>
#include <math.h>
#include <Logger.h>

float Util::calculateDistance(int x1, int y1, int x2, int y2)
{
    Logger::debug("Util::calculateDistance");
    return sqrt(
        (x2 - x1) * (x2 - x1) +
        (y2 - y1) * (y2 - y1)
    );
}

vector<SDL_Point> Util::getCirclePoints(int centerX, int centerY, int radius)
{
    vector<SDL_Point> points;

    int x = radius;
    int y = 0;
    int XChange = 1 - 2 * radius;
    int YChange = 1;
    int error = 0;
    while (x >= y) {
        points.push_back({centerX - x, centerY + y});
        points.push_back({centerX - x, centerY - y});
        points.push_back({centerX + x, centerY - y});
        points.push_back({centerX + x, centerY + y});

        points.push_back({centerX - y, centerY - x});
        points.push_back({centerX - y, centerY + x});
        points.push_back({centerX + y, centerY - x});
        points.push_back({centerX + y, centerY + x});

        y++;
        error += YChange;
        YChange += 2;

        if (2 * error + XChange > 0) {
            x--;
            error += XChange;
            XChange += 2;
        }
    }
    return points;
}

SDL_Texture* Util::createBorderedCircleTexture(SDL_Renderer *renderer, int radius, int border,
                                       vector<int> circleColor, vector<int> borderColor)
{
    SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                         SDL_TEXTUREACCESS_TARGET, 2*radius+1, 2*radius+1);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    vector<SDL_Point> circlePoints = getCirclePoints(0, 0, radius);
    vector<SDL_Point> innerCirclePoints = getCirclePoints(0, 0, radius - border);

    SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
    for (size_t i=0; i < circlePoints.size() - 1; i += 2) {
        SDL_RenderDrawLine(renderer,
                           circlePoints[i].x + radius,
                           circlePoints[i].y + radius,
                           circlePoints[i+1].x + radius,
                           circlePoints[i+1].y + radius);
    }

    SDL_SetRenderDrawColor(renderer, circleColor[0], circleColor[1], circleColor[2], circleColor[3]);
    for (size_t i=0; i < innerCirclePoints.size() - 1; i += 2) {
        SDL_RenderDrawLine(renderer,
                           innerCirclePoints[i].x + radius,
                           innerCirclePoints[i].y + radius,
                           innerCirclePoints[i+1].x + radius,
                           innerCirclePoints[i+1].y + radius);
    }

    SDL_SetRenderTarget(renderer, NULL);
    return tex;
}

void Util::drawText(SDL_Renderer* renderer, SDL_Texture* messageTexture,
                    SDL_Rect messageRect)
{
    Logger::debug("Util::drawText");
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
}

void Util::drawLine(SDL_Renderer* renderer, SDL_Texture* lineTexture,
                    int x1, int y1, int x2, int y2,
                    int width)
{
    Logger::debug("Util::drawLine");
    double distance = calculateDistance(x1, y1, x2, y2);
    double angle = 180 - 180 / 3.1415926 * atan2(x2 - x1, y2 - y1);

    SDL_Rect rect;
    rect.x = x1 + (x2 - x1)/2 + width/2;
    rect.y = y1 + (y2 - y1)/2 - distance/2;
    rect.w = width;
    rect.h = distance;

    SDL_RenderCopyEx(renderer, lineTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void Util::drawRectWithBorder(SDL_Renderer* renderer,
                              SDL_Rect rect, int borderWidth,
                              vector<int> rectColor,
                              vector<int> borderColor)
{
    Logger::debug("Util::drawRectWithBorder");

    if (rectColor.size() != 4 || borderColor.size() != 4)
        throw std::runtime_error("Util::drawRectWithBorder - invalid color");

    SDL_SetRenderDrawColor(renderer, borderColor[0],
                           borderColor[1], borderColor[2],
                           borderColor[3]);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Rect bodyRect;
    bodyRect.x = rect.x + borderWidth/2;
    bodyRect.y = rect.y + borderWidth/2;
    bodyRect.w = rect.w - borderWidth;
    bodyRect.h = rect.h - borderWidth;

    SDL_SetRenderDrawColor(renderer, rectColor[0],
                           rectColor[1], rectColor[2],
                           rectColor[3]);
    SDL_RenderFillRect(renderer, &bodyRect);
}


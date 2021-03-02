#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include <Graph/Vertex.h>

using std::string;
using std::vector;

class Util
{
    public:
        static float calculateDistance(int x1, int y1, int x2, int y2);
        static vector<SDL_Point> getCirclePoints(int centerX, int centerY, int radius);
        static SDL_Texture* createBorderedCircleTexture(SDL_Renderer* renderer, int radius, int border,
                                                vector<int> circleColor, vector<int> borderColor);
        static void drawText(SDL_Renderer* renderer, SDL_Texture* messageTexture,
                             SDL_Rect messageRect);
        static void drawLine(SDL_Renderer* renderer, SDL_Texture* lineTexture,
                             int x1, int y1, int x2, int y2,
                             int width);
        static void drawRectWithBorder(SDL_Renderer* renderer,
                                       SDL_Rect rect, int borderWidth,
                                       vector<int> rectColor,
                                       vector<int> borderColor);
};


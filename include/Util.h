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
        static double calculateDistance(int x1, int y1, int x2, int y2);
        static double calculateDistanceF(double x1, double y1, double x2, double y2);
        static vector<SDL_Point> getCirclePoints(int centerX, int centerY, int radius);
        static SDL_Texture* createBorderedCircleTexture(SDL_Renderer* renderer, int radius, int border,
                                                vector<int> circleColor, vector<int> borderColor);
        static void drawText(SDL_Renderer* renderer, SDL_Texture* messageTexture,
                             SDL_Rect messageRect);
        static void drawLine(SDL_Renderer* renderer, SDL_Texture* lineTexture,
                             int x1, int y1, int x2, int y2,
                             int width);
        static void drawLineF(SDL_Renderer* renderer, SDL_Texture* lineTexture,
                             double x1, double y1, double x2, double y2,
                             int width);
        static void drawCurvedLine(SDL_Renderer* renderer,
                                   SDL_Texture* lineTexture,
                                   vector<int>& vx, vector<int>& vy,
                                   int steps, int width);
        static SDL_FPoint getCurvedLinePoint(vector<int>& vx, vector<int>& vy, double t);
        static void drawRectWithBorder(SDL_Renderer* renderer,
                                       SDL_Rect rect, int borderWidth,
                                       vector<int> rectColor,
                                       vector<int> borderColor);
};


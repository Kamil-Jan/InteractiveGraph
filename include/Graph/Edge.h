#pragma once
#include <vector>
#include <Graph/Vertex.h>

class Edge
{
    private:
        Vertex* start;
        Vertex* end;
        SDL_Texture* lineTexture;
        SDL_Texture* weightTexture;
        SDL_Rect weightRect;
        int curvePointX = -1;
        int curvePointY = -1;
        int weight;
        bool directed;
        bool curved = false;

    public:
        Edge(Vertex* start, Vertex* end,
             int weight, bool directed,
             SDL_Texture* lineTexture);

        static SDL_Texture* createWeightTexture(int weight);
        static SDL_Rect createWeightRect(SDL_Texture* texture);

        Vertex* getStart();
        Vertex* getEnd();
        SDL_Texture* getWeightTexture();
        SDL_Rect getWeightRect();
        int getWeight();
        void setWeight(int weight);
        void setWeightTexture(SDL_Texture* texture);
        void setLineTexture(SDL_Texture* texture);
        void setWeightRect(SDL_Rect rect);
        void setCurved();
        void resetCurved();
        void setCurvePoint(int x, int y);

        bool isDirected();

        void drawWeight(SDL_Renderer* renderer);
        void drawArrows(SDL_Renderer* renderer, int lineWidth);
        void drawEdge(SDL_Renderer* renderer, int lineWidth);

        bool isInside(int mouseX, int mouseY);
        void render();
};


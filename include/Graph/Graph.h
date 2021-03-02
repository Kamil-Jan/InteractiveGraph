#pragma once
#include <string>
#include <vector>
#include <map>

#include <Graph/Vertex.h>

using std::string;
using std::pair;
using std::map;

class Graph
{
    private:
        static Graph* instance;
        map<int, Vertex*> graphMap;
        map<pair<Vertex*, Vertex*>, SDL_Texture*> connectionsTexMap;
        map<pair<Vertex*, Vertex*>, SDL_Rect*> connectionsRectMap;
        int counter = 0;

    public:
        Graph();
        ~Graph();
        static Graph* getInstance();

        void setCounter(int counter);
        int getCounter();

        map<pair<Vertex*, Vertex*>, SDL_Rect*> getConnectionsRectMap();

        void addVertex(Vertex* vertex);
        void removeVertex(Vertex* vertex);
        bool hasVertex(Vertex* vertex);
        Vertex* getVertex(int vertexId);
        vector<Vertex*> getVertices();

        void addEdge(Vertex* start, Vertex* end, int weight=1);
        void removeEdge(Vertex* start, Vertex* end);
        bool hasEdge(Vertex* start, Vertex* end);
        void removeEdgesTo(Vertex* vertex);
        void clear();

        vector<Vertex*> findTheShortestPath(Vertex* start, Vertex* end);

        SDL_Texture* createWeightTexture(int weight);
        SDL_Rect* createWeightRect(SDL_Texture* texture);
        void setWeightTexture(Vertex* start, Vertex* end, SDL_Texture* texture);
        void setWeightRect(Vertex* start, Vertex* end, SDL_Rect* rect);
        void drawWeight(SDL_Renderer* renderer, Vertex* start, Vertex* end);
        void drawArrows(SDL_Renderer* renderer, SDL_Texture* edgeTexture,
                        Vertex* start, Vertex* end,
                        int edgeWidth);
        void drawEdge(SDL_Renderer* renderer, SDL_Texture* edgeTexture,
                      Vertex* start, Vertex* end,
                      int edgeWidth);
        void drawPath(SDL_Renderer* renderer, int edgeWidth, vector<Vertex*> path);

        void update(float ticks);
        void render();
};


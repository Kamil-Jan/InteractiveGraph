#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <Graph/Vertex.h>
#include <Graph/Edge.h>

using std::string;
using std::pair;
using std::map;
using std::unordered_map;

class Graph
{
    private:
        static Graph* instance;
        map<int, Vertex*> graphMap;
        map<std::pair<Vertex*, Vertex*>, Edge*> edgesMap;
        int counter = 0;
        bool directed = true;

    public:
        Graph();
        ~Graph();
        static Graph* getInstance();

        void setCounter(int counter);
        int getCounter();

        void setDirected();
        void setUndirected();

        void addVertex(Vertex* vertex);
        void removeVertex(Vertex* vertex);
        bool hasVertex(Vertex* vertex);
        Vertex* getVertex(int vertexId);
        vector<Vertex*> getVertices();
        int getVerticesCount();

        void addEdge(Vertex* start, Vertex* end, int weight=1);
        Edge* getEdge(Vertex* start, Vertex* end);
        void setEdgeWeight(Vertex* start, Vertex* end, int weight);
        void removeEdge(Vertex* start, Vertex* end);
        void removeAdjacentEdges(Vertex* vertex);
        bool hasEdge(Vertex* start, Vertex* end);

        void clear();

        vector<Vertex*> findTheShortestPath(Vertex* start, Vertex* end);
        vector<Vertex*> findArticulationPoints(Vertex* root, int d,
                                               unordered_map<Vertex*, int>& depth,
                                               unordered_map<Vertex*, int>& low,
                                               unordered_map<Vertex*, bool>& visited,
                                               unordered_map<Vertex*, Vertex*>& parent);
        vector<Vertex*> findArticulationPoints();

        void update(float ticks);
        void render();
};


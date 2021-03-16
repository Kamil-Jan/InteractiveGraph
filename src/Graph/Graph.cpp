#include <iostream>
#include <algorithm>
#include <math.h>

#include <Logger.h>
#include <Util.h>
#include <Game.h>
#include <Graph/Graph.h>

Graph* Graph::instance = NULL;

Graph::Graph()
{
    instance = instance? instance : this;
}

Graph::~Graph()
{
    Logger::debug("Graph::~Graph");
}

Graph* Graph::getInstance()
{
    return instance;
}

int Graph::getCounter()
{
    return counter;
}

void Graph::setCounter(int counter)
{
    if (counter >= 0)
        this->counter = counter;
    else
        throw std::runtime_error("Graph::setCounter - invalid counter");
}

void Graph::setDirected()
{
    directed = true;
}

void Graph::setUndirected()
{
    directed = false;
}

void Graph::addVertex(Vertex* vertex)
{
    Logger::debug("Graph::addVertex");
    if (!hasVertex(vertex)) {
        graphMap[vertex->getId()] = vertex;
        counter++;
    }
}

void Graph::removeVertex(Vertex* vertex)
{
    Logger::debug("Graph::removeVertex");
    removeAdjacentEdges(vertex);
    graphMap.erase(vertex->getId());
}

bool Graph::hasVertex(Vertex* vertex)
{
    Logger::debug("Graph::hasVertex");
    return graphMap.find(vertex->getId()) != graphMap.end();
}

Vertex* Graph::getVertex(int vertexId)
{
    Logger::debug("Graph::getVertex");
    return graphMap[vertexId];
}

vector<Vertex*> Graph::getVertices()
{
    Logger::debug("Graph::getVertices");
    vector<Vertex*> vertices;
    for (auto kv : graphMap)
        vertices.push_back(kv.second);
    return vertices;
}

int Graph::getVerticesCount()
{
    Logger::debug("Graph::getVerticesCount");
    return graphMap.size();
}

void Graph::addEdge(Vertex* start, Vertex* end, int weight)
{
    Logger::debug("Graph::addEdge");
    addVertex(start);
    addVertex(end);
    start->addNeighbor(end, weight);

    Edge* edge = new Edge(start, end, weight, directed,
                          Game::getInstance()->getEdgeTexture());
    edgesMap[std::make_pair(start, end)] = edge;

    if (!directed) {
        end->addNeighbor(start, weight);
        edgesMap[std::make_pair(end, start)] = edge;
    }
}

Edge* Graph::getEdge(Vertex* start, Vertex* end)
{
    return edgesMap[std::make_pair(start, end)];
}

void Graph::setEdgeWeight(Vertex* start, Vertex* end, int weight)
{
    start->setNeighborWeight(end, weight);
    Edge* edge = edgesMap[std::make_pair(start, end)];
    edge->setWeight(weight);
    if (!edge->isDirected()) {
        end->setNeighborWeight(start, weight);
        edgesMap[std::make_pair(end, start)]->setWeight(weight);
    }
}

void Graph::removeEdge(Vertex* start, Vertex* end)
{
    Logger::debug("Graph::removeEdge");
    if (start->hasNeighbor(end)) {
        start->removeNeighbor(end);
        if (!edgesMap[std::make_pair(start, end)]->isDirected()) {
            end->removeNeighbor(start);
            edgesMap.erase(std::make_pair(end, start));
        }
        edgesMap.erase(std::make_pair(start, end));
    }
}

bool Graph::hasEdge(Vertex* start, Vertex* end)
{
    Logger::debug("Graph::hasEdge");
    return start->hasNeighbor(end);
}

void Graph::removeAdjacentEdges(Vertex* vertex)
{
    Logger::debug("Graph::removeEdgesTo");
    for (auto kv : graphMap) {
        removeEdge(kv.second, vertex);
        removeEdge(vertex, kv.second);
    }
}

void Graph::clear()
{
    Logger::debug("Graph::clear");
    graphMap.clear();
    edgesMap.clear();
    counter = 0;
}

vector<Vertex*> Graph::findTheShortestPath(Vertex* start, Vertex* end)
{
    Logger::debug("Graph::findTheShortestPath");
    if (!hasVertex(start) || !hasVertex(end))
        throw std::runtime_error("Graph::findTheShortestPath - invalid argument vertex");

    int inf = INT32_MAX;
    vector<Vertex*> vertices = getVertices();

    unordered_map<Vertex*, int> distances;
    unordered_map<Vertex*, Vertex*> prevVertices;
    for (Vertex* vertex : vertices) {
        distances[vertex] = inf;
        prevVertices[vertex] = NULL;
    }
    distances[start] = 0;

    auto comp = [&](Vertex* x, Vertex* y) {
        return (distances[x] > distances[y]);
    };

    std::make_heap(vertices.begin(), vertices.end(), comp);
    while (!vertices.empty()) {
        Vertex* curVertex = vertices.front();
        std::pop_heap(vertices.begin(), vertices.end(), comp);
        vertices.pop_back();

        if (distances[curVertex] == inf) break;

        for (Vertex* neighbor : curVertex->getConnections()) {
            int weight = curVertex->getNeighborWeight(neighbor);
            int altRoutDistance = distances[curVertex] + weight;
            if (altRoutDistance < distances[neighbor]) {
                distances[neighbor] = altRoutDistance;
                prevVertices[neighbor] = curVertex;
            }
        }

        std::make_heap(vertices.begin(), vertices.end(), comp);
    }

    vector<Vertex*> path;
    Vertex* curVertex = end;
    while (curVertex != NULL) {
        path.insert(path.begin(), curVertex);
        curVertex = prevVertices[curVertex];
    }
    return path;
}

vector<Vertex*> Graph::findArticulationPoints(Vertex* root, int d,
                                              unordered_map<Vertex*, int>& depth,
                                              unordered_map<Vertex*, int>& low,
                                              unordered_map<Vertex*, bool>& visited,
                                              unordered_map<Vertex*, Vertex*>& parent)
{
    vector<Vertex*> artPoints;

    visited[root] = true;
    depth[root] = d;
    low[root] = d;

    int childrenCount = 0;
    for (Vertex* neighbor : root->getConnections()) {
        if (!visited[neighbor]) {
            parent[neighbor] = root;
            childrenCount++;

            vector<Vertex*> addedArtPoints = findArticulationPoints(
                neighbor, d + 1, depth, low, visited, parent
            );

            artPoints.reserve(artPoints.size() + addedArtPoints.size());
            artPoints.insert(artPoints.end(), addedArtPoints.begin(), addedArtPoints.end());

            low[root] = (low[root] < low[neighbor])? low[root] : low[neighbor];
            if (parent[root] && low[neighbor] >= depth[root] &&
                find(artPoints.begin(), artPoints.end(), root) == artPoints.end())
                    artPoints.push_back(root);
        }
        else if (neighbor != parent[root])
            low[root] = (low[root] < depth[neighbor])? low[root] : depth[neighbor];
    }

    if (d == 1 && childrenCount > 1)
        artPoints.push_back(root);

    return artPoints;
}

vector<Vertex*> Graph::findArticulationPoints()
{
    unordered_map<Vertex*, int> depth;
    unordered_map<Vertex*, int> low;
    unordered_map<Vertex*, bool> visited;
    unordered_map<Vertex*, Vertex*> parent;

    vector<Vertex*> vertices = getVertices();
    for (Vertex* vertex : vertices) {
        visited[vertex] = false;
        parent[vertex] = NULL;
    }

    return findArticulationPoints(vertices[0], 1, depth, low, visited, parent);
}

void Graph::update(float ticks)
{
    Logger::debug("Graph::update");
    for (auto kv : graphMap)
        kv.second->update(ticks);
}

void Graph::render()
{
    Logger::debug("Graph::render");

    for (auto kv : edgesMap) {
        Vertex* start = kv.first.first;
        Vertex* end = kv.first.second;
        if (!kv.second->isDirected() && start->getId() > end->getId())
            continue;
        kv.second->render();
    }

    for (auto kv : graphMap) kv.second->render();
}


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
    for (auto kv : connectionsTexMap) {
        SDL_DestroyTexture(kv.second);
    }
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

map<std::pair<Vertex*, Vertex*>, SDL_Rect*> Graph::getConnectionsRectMap()
{
    return connectionsRectMap;
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
    removeEdgesTo(vertex);
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

void Graph::addEdge(Vertex* start, Vertex* end, int weight)
{
    Logger::debug("Graph::addEdge");
    addVertex(start);
    addVertex(end);
    start->addNeighbor(end, weight);

    SDL_Texture* texture = createWeightTexture(weight);
    connectionsTexMap[std::make_pair(start, end)] = texture;
    connectionsRectMap[std::make_pair(start, end)] =
        createWeightRect(texture);
}

void Graph::removeEdge(Vertex* start, Vertex* end)
{
    Logger::debug("Graph::removeEdge");
    if (start->hasNeighbor(end)) {
        start->removeNeighbor(end);
        connectionsTexMap.erase(std::make_pair(start, end));
        connectionsRectMap.erase(std::make_pair(start, end));
    }
}

bool Graph::hasEdge(Vertex* start, Vertex* end)
{
    Logger::debug("Graph::hasEdge");
    return start->hasNeighbor(end);
}

void Graph::removeEdgesTo(Vertex* vertex)
{
    Logger::debug("Graph::removeEdgesTo");
    for (auto kv : graphMap)
        removeEdge(kv.second, vertex);
}

void Graph::clear()
{
    Logger::debug("Graph::clear");
    graphMap.clear();
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

SDL_Texture* Graph::createWeightTexture(int weight)
{
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

SDL_Rect* Graph::createWeightRect(SDL_Texture* texture)
{
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect* weightRect = new SDL_Rect;
    weightRect->w = width;
    weightRect->h = height;
    return weightRect;
}

void Graph::setWeightTexture(Vertex* start, Vertex* end, SDL_Texture* texture)
{
    connectionsTexMap[std::make_pair(start, end)] = texture;
}

void Graph::setWeightRect(Vertex* start, Vertex* end, SDL_Rect* rect)
{
    connectionsRectMap[std::make_pair(start, end)] = rect;
}

void Graph::drawWeight(SDL_Renderer* renderer, Vertex* start, Vertex* end)
{
    int x1 = start->getX();
    int y1 = start->getY();
    int x2 = end->getX();
    int y2 = end->getY();

    SDL_Rect* weightRect = connectionsRectMap[std::make_pair(start, end)];
    weightRect->x = x1 + (x2 - x1)/2 - weightRect->w/2;
    weightRect->y = y1 + (y2 - y1)/2 - weightRect->h/2;

    int angle = 90 - 180 / 3.1415926 * atan2(x2 - x1, y2 - y1);
    if (angle > 90)
        angle += 180;

    SDL_RenderCopyEx(
        renderer,
        connectionsTexMap[std::make_pair(start, end)],
        NULL, weightRect,
        angle,
        NULL, SDL_FLIP_NONE
    );
}

void Graph::drawArrows(SDL_Renderer* renderer, SDL_Texture* edgeTexture, Vertex* start, Vertex* end, int edgeWidth)
{
    int x1 = start->getX();
    int y1 = start->getY();
    int x2 = end->getX();
    int y2 = end->getY();

    float distance = Util::calculateDistance(x1, y1, x2, y2);
    int deltaX = (Game::VERTEX_RADIUS * (x1 - x2)) / (distance);
    int deltaY = (Game::VERTEX_RADIUS * (y1 - y2)) / (distance);
    int interceptX = x2 + deltaX;
    int interceptY = y2 + deltaY;

    float edgeAngle = atan((float) (y2 - y1) / (x2 - x1));
    float alpha = edgeAngle + Game::ARROW_LINE_ANGLE;
    float betha = edgeAngle - Game::ARROW_LINE_ANGLE;

    int line1X, line1Y, line2X, line2Y;
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

    Util::drawLine(renderer, edgeTexture,
                   interceptX, interceptY,
                   line1X, line1Y,
                   edgeWidth);
    Util::drawLine(renderer, edgeTexture,
                   interceptX, interceptY,
                   line2X, line2Y,
                   edgeWidth);
}

void Graph::drawEdge(SDL_Renderer* renderer, SDL_Texture* edgeTexture,
                     Vertex* start, Vertex* end,
                     int edgeWidth)
{
    Logger::debug("Graph::drawEdge");

    int x1 = start->getX();
    int y1 = start->getY();
    int x2 = end->getX();
    int y2 = end->getY();

    Util::drawLine(renderer, edgeTexture, x1, y1, x2, y2, edgeWidth);
    drawArrows(renderer, edgeTexture, start, end, edgeWidth);
    drawWeight(renderer, start, end);
}

void Graph::drawPath(SDL_Renderer* renderer, int edgeWidth, vector<Vertex*> path)
{
    Logger::debug("Graph::drawPath");
    for (size_t i=0; i < path.size() - 1; i++)
        drawEdge(renderer,
                 Game::getInstance()->getPathEdgeTexture(),
                 path[i], path[i + 1], edgeWidth);

    for (Vertex* vertex : path)
        vertex->render();
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
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();

    for (auto kv : graphMap) {
        Vertex* vertex = kv.second;
        for (Vertex* neighbor : vertex->getConnections())
            drawEdge(renderer,
                     Game::getInstance()->getEdgeTexture(),
                     vertex, neighbor, Game::EDGE_WIDTH);
    }

    for (auto kv : graphMap) kv.second->render();
}


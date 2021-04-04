#include <iostream>
#include <ostream>

#include <Game.h>
#include <Util.h>
#include <Logger.h>
#include <Graph/Vertex.h>

Vertex::Vertex(int id, int x, int y, int radius, int border) :
    Sprite(x, y, radius, radius)
{
    Logger::debug("Vertex::Vertex");

    setId(id);
    this->radius = radius;
    this->circleBorder = border;

    SDL_Surface* idSurface = TTF_RenderText_Blended(
        Game::getInstance()->getFont(),
        std::to_string(id).c_str(),
        { 255, 255, 255, 255 }
    );
    idTexture = SDL_CreateTextureFromSurface(
        Game::getInstance()->getRenderer(),
        idSurface
    );
    idRect.w = idSurface->w;
    idRect.h = idSurface->h;
    SDL_FreeSurface(idSurface);
}

Vertex::~Vertex()
{
    Logger::debug("Vertex::~Vertex");
    SDL_DestroyTexture(idTexture);
}

void Vertex::setId(int id)
{
    if (id > 0)
        this->id = id;
    else
        throw std::runtime_error("Vertex::Vertex - invalid id");
}

void Vertex::setSelected()
{
    selected = true;
}

void Vertex::setNeighborWeight(Vertex* neighbor, int weight)
{
    adjacent[neighbor] = weight;
}

void Vertex::resetSelected()
{
    selected = false;
}

int Vertex::getId()
{
    return id;
}

void Vertex::addNeighbor(Vertex* neighbor, int weight)
{
    Logger::debug("Vertex::addNeighbor");
    adjacent[neighbor] = weight;
}

void Vertex::removeNeighbor(Vertex *neighbor)
{
    Logger::debug("Vertex::removeNeighbor");
    adjacent.erase(neighbor);
}

bool Vertex::hasNeighbor(Vertex* neighbor)
{
    Logger::debug("Vertex::hasNeighbor");
    return (adjacent.find(neighbor) != adjacent.end());
}

vector<Vertex*> Vertex::getConnections()
{
    Logger::debug("Vertex::getConnections");
    std::vector<Vertex*> keys;

    for (auto keyValuePair : adjacent) {
        keys.push_back(keyValuePair.first);
    }

    return keys;
}

int Vertex::getNeighborWeight(Vertex* neighbor)
{
    Logger::debug("Vertex::getNeighborWeight");
    if (adjacent.find(neighbor) != adjacent.end())
        return adjacent[neighbor];
    throw std::runtime_error("Vertex::getNeighborWeight: invalid neighbor");
}

bool Vertex::isInside(int x, int y)
{
    Logger::debug("Vertex::isInside");
    return (Util::calculateDistance(getX(), getY(), x, y) <= (double) radius);
}

void Vertex::update(float ticks)
{
    Logger::debug("Vertex::update");
}

void Vertex::render()
{
    Logger::debug("Vertex::render");
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();

    SDL_Rect vertexRect;
    vertexRect.x = getX() - radius;
    vertexRect.y = getY() - radius;
    vertexRect.w = 2 * radius;
    vertexRect.h = 2 * radius;

    if (selected)
        SDL_RenderCopy(renderer,
                       Game::getInstance()->getSelectedVertexTexture(),
                       NULL, &vertexRect);
    else
        SDL_RenderCopy(renderer,
                       Game::getInstance()->getVertexTexture(),
                       NULL, &vertexRect);

    idRect.x = getX() - idRect.w/2;
    idRect.y = getY() - idRect.h/2;
    Util::drawText(renderer, idTexture, idRect);
}

std::ostream& operator<<(std::ostream& outs, Vertex& vertex)
{
    return outs << "Vertex: id=" + std::to_string(vertex.getId());
}

std::ostream& operator<<(std::ostream& outs, Vertex* vertex)
{
    return outs << "Vertex: id=" + std::to_string(vertex->getId());
}


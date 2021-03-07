#pragma once
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <ostream>
#include <vector>

#include <Sprite/Sprite.h>

using std::unordered_map;
using std::vector;
using std::vector;

class Vertex : public Sprite
{
    private:
        int id;
        int radius;
        int circleBorder;
        unordered_map<Vertex*, int> adjacent;
        bool selected = false;

        SDL_Texture* idTexture;
        SDL_Rect idRect;

    public:
        Vertex(int id, int x, int y, int radius, int border);
        ~Vertex();

        void setId(int id);
        void setSelected();
        void resetSelected();

        int getId();
        bool isSelected();
        bool isPathVertex();

        void addNeighbor(Vertex* neighbor, int weight=1);
        void removeNeighbor(Vertex* neighbor);
        bool hasNeighbor(Vertex* neighbor);
        vector<Vertex*> getConnections();
        int getNeighborWeight(Vertex* neighbor);

        bool isInside(int x, int y) override;
        void update(float ticks) override;
        void render() override;
};

std::ostream& operator<<(std::ostream& outs, Vertex& vertex);
std::ostream& operator<<(std::ostream& outs, Vertex* vertex);


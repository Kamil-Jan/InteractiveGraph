#pragma once
#include <Sprite/SpriteGroup.h>
#include <Graph/Vertex.h>
#include <Mode/Mode.h>

class ShortestPathMode : public Mode
{
    private:
        static ShortestPathMode* instance;
        Vertex* selectedVertex = NULL;
        vector<Vertex*> path;

    public:
        ShortestPathMode();
        static ShortestPathMode* getInstance();

        void resetSelectedVertex();
        void resetPath();

        void update(float ticks) override;
        void render() override;
        void quit() override;
};


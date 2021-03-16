#pragma once
#include <Mode/Mode.h>
#include <Graph/Vertex.h>

class ArticulationPointsMode : public Mode
{
    private:
        vector<Vertex*> artPoints;

    public:
        void update(float ticks) override;
        void render() override;
        void quit() override;
};


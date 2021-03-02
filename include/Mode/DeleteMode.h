#pragma once
#include <Mode/Mode.h>
#include <Graph/Vertex.h>

class DeleteMode : public Mode
{
    private:
        Vertex* selectedVertex = NULL;

    public:
        void manageLeftMouseButtonClick(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse);
        void manageSelectedVertex(vector<Vertex*> spritesUnderMouse);
        void update(float ticks) override;
        void render() override;
        void quit() override;
};


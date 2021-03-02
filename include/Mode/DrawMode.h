#pragma once
#include <Mode/Mode.h>
#include <Graph/Vertex.h>

class DrawMode : public Mode
{
    private:
        Vertex* draggedVertex = NULL;
        Vertex* selectedVertex = NULL;

    public:
        DrawMode();

        void manageLeftMouseButtonClick(int mouseX, int mouseY,
                                        vector<Vertex*> spritesUnderMouse);
        void manageSelectedVertex(vector<Vertex*> spritesUnderMouse);
        void manageDraggedVertex(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse);
        void update(float ticks) override;
        void render() override;
        void quit() override;
};


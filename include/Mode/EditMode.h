#pragma once
#include <Graph/Edge.h>
#include <Mode/Mode.h>

class EditMode : public Mode
{
    private:
        bool edit = false;
        std::string weightStr;
        Edge* editableEdge;
        Vertex* selectedVertex = NULL;
        SDL_Texture* editTex;

    public:
        EditMode();
        ~EditMode();
        void manageLeftMouseButtonClick(int mouseX, int mouseY, vector<Vertex*> spritesUnderMouse);
        void manageSelectedVertex(vector<Vertex*> spritesUnderMouse);
        void getUserInput();
        void update(float ticks) override;
        void render() override;
        void quit() override;
};


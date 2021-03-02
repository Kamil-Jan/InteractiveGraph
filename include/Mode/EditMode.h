#pragma once
#include <Graph/Vertex.h>
#include <Mode/Mode.h>

class EditMode : public Mode
{
    private:
        bool edit = false;
        std::string weightStr;
        std::pair<Vertex*, Vertex*> editableEdge;
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


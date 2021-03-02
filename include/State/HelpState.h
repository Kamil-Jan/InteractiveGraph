#pragma once
#include <SDL2/SDL_image.h>
#include <State/State.h>
#include <GUI/Widget/Button.h>

class HelpState : public State
{
    private:
        SDL_Texture* shortcutsImg = NULL;
        SDL_Rect shortcutsRect;

        Button* backButton;

    public:
        HelpState();
        ~HelpState();

        void update() override;
        void render() override;
};


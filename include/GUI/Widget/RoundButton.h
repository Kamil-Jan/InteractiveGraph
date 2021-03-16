#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <GUI/Widget/Button.h>

class RoundButton : public Button
{
    private:
        SDL_Texture* circleTex;
        SDL_Texture* selectedCircleTex;
        SDL_Texture* innerContentTex;
        int radius;

    public:
        RoundButton(SDL_Texture* innerContentTex,
                    Action* action, int x, int y,
                    int radius, int borderWidth,
                    vector<int> bgColor,
                    vector<int> selectedBgColor,
                    vector<int> borderColor);
        ~RoundButton();

        bool isInside(int x, int y) override;
        void render() override;
};


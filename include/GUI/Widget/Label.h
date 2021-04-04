#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <GUI/Widget/Widget.h>

class Label : public Widget
{
    protected:
        std::string text;
        std::vector<int> fgColor;
        std::vector<int> bgColor;
        std::vector<int> borderColor;
        int borderWidth;
        TTF_Font* font;
        SDL_Texture* textTexture = NULL;
        SDL_Rect textRect;

    public:
        Label(std::string text, TTF_Font* font,
              std::vector<int> fgColor,
              int x, int y, int w, int h);
        Label(std::string text, TTF_Font* font,
              std::vector<int> fgColor,
              int x, int y, int w, int h,
              int borderWidth, std::vector<int> bgColor,
              std::vector<int> borderColor);
        ~Label();

        std::string getText();
        bool isInside(int x, int y) override;
        void reset() override;
        void generateTextTexture();
        void update(float ticks) override;
        void render() override;
};


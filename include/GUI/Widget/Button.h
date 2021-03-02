#pragma once
#include <vector>
#include <string>
#include <Action/Action.h>
#include <GUI/Widget/Widget.h>

using std::vector;
using std::string;

class Button : public Widget
{
    protected:
        vector<int> curColor;
        vector<int> bgColor;
        vector<int> selectedBgColor;
        vector<int> borderColor;
        int borderWidth;
        bool selected = false;
        string text;
        Action* action;
        SDL_Texture* textTexture = NULL;
        SDL_Rect textRect;

    public:
        Button(string text, Action* action,
               int x, int y, int h, int w,
               int borderWidth, vector<int> bgColor,
               vector<int> selectedBgColor,
               vector<int> borderColor);
        ~Button();

        string getText();
        bool getSelected();
        void setSelected();

        void reset() override;
        bool isInside(int x, int y) override;
        void update(float ticks) override;
        void render() override;
};


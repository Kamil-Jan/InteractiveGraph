#pragma once
#include <vector>
#include <GUI/Widget/Widget.h>
#include <Sprite/Sprite.h>
#include <Sprite/SpriteGroup.h>

class Menu : public Sprite
{
    protected:
        vector<int> bgColor;
        vector<int> borderColor;
        int borderWidth;

        SpriteGroup<Widget> widgets;

    public:
        Menu(int x, int y, int width, int height,
             int borderWidth, vector<int> bgColor,
             vector<int> borderColor);
        void addWidget(Widget* widget);
        bool isInside(int x, int y);
        bool isInsideWidgets(int x, int y);
        bool isAround(int x, int y);
        virtual void update(float ticks);
        virtual void render();
};


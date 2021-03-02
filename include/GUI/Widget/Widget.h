#pragma once
#include <Sprite/Sprite.h>

class Widget : public Sprite
{
    public:
        Widget(int x, int y, int w, int h);
        virtual void reset() = 0;
};


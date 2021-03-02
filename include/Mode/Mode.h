#pragma once
#include <vector>
#include <GUI/Widget/Widget.h>
#include <Sprite/Sprite.h>
#include <Sprite/SpriteGroup.h>

using std::vector;

class Mode
{
    public:
        Mode();
        virtual void quit() = 0;
        virtual void update(float ticks) = 0;
        virtual void render() = 0;
};


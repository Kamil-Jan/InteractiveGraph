#pragma once
#include <SDL2/SDL.h>
#include <memory>

using std::shared_ptr;

class Sprite
{
    protected:
        shared_ptr<SDL_Texture> tex = NULL;
        SDL_Rect rect;

    public:
        Sprite(int x, int y, int w, int h);

        virtual bool isInside(int x, int y) = 0;
        virtual void update(float ticks) = 0;
        virtual void render() = 0;

        void setPosition(int x, int y);
        int getX();
        int getY();
        int getWidth();
        int getHeight();

        SDL_Texture* getTexture() const;
        bool operator==(const Sprite& other) const;
};


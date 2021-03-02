#include <Sprite/Sprite.h>
#include <Logger.h>

Sprite::Sprite(int x, int y, int w, int h)
{
    Logger::debug("Sprite::Sprite");
    rect.w = w;
    rect.h = h;
    setPosition(x, y);
}

void Sprite::setPosition(int x, int y)
{
    Logger::debug("Sprite::setPosition");
    rect.x = x;
    rect.y = y;
}

int Sprite::getX()
{
    return rect.x;
}

int Sprite::getY()
{
    return rect.y;
}

int Sprite::getWidth()
{
    return rect.w;
}

int Sprite::getHeight()
{
    return rect.h;
}

SDL_Texture* Sprite::getTexture() const
{
    return tex.get();
}

bool Sprite::operator==(const Sprite& other) const
{
    return (tex.get() == other.getTexture());
}


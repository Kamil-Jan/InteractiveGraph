#include <Logger.h>
#include <GUI/Widget/Widget.h>
#include <Graph/Vertex.h>
#include <Sprite/SpriteGroup.h>

template<class T>
int SpriteGroup<T>::size()
{
    return spritesSize;
}

template<class T>
vector<T*> SpriteGroup<T>::getSprites()
{
    return sprites;
}

template<class T>
T* SpriteGroup<T>::get(int index)
{
    return sprites[index];
}

template <class T>
SpriteGroup<T> SpriteGroup<T>::copy()
{
    Logger::debug("SpriteGroup::copy");
    SpriteGroup newGroup;
    for (int i=0; i < spritesSize; i++) {
        newGroup.add(sprites[i]);
    }
    return newGroup;
}

template <class T>
void SpriteGroup<T>::add(T* sprite)
{
    Logger::debug("SpriteGroup::add");
    sprites.push_back(sprite);
    spritesSize = sprites.size();
}

template <class T>
void SpriteGroup<T>::remove(T* sprite)
{
    Logger::debug("SpriteGroup::remove");
    for (int i=0; i < spritesSize; i++)
        if (sprites[i] == sprite)
            sprites.erase(sprites.begin() + i);

    spritesSize = sprites.size();
}

template <class T>
bool SpriteGroup<T>::has(T* sprite)
{
    Logger::debug("SpriteGroup::has");
    for (int i=0; i < spritesSize; i++)
        if (sprites[i] == sprite) return true;
    return false;
}

template <class T>
void SpriteGroup<T>::clear()
{
    Logger::debug("SpriteGroup::clear");
    sprites.clear();
    spritesSize = sprites.size();
}

template <class T>
void SpriteGroup<T>::update(float ticks)
{
    Logger::debug("SpriteGroup::update");
    if (sprites.empty())
        return;
    for (int i=0; i < spritesSize; i++)
        sprites[i]->update(ticks);
}

template <class T>
void SpriteGroup<T>::render()
{
    Logger::debug("SpriteGroup::render");
    if (sprites.empty())
        return;

    for (int i=0; i < spritesSize; i++)
        sprites[i]->render();
}

template class SpriteGroup<Widget>;
template class SpriteGroup<Vertex>;


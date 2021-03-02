#pragma once
#include <vector>

using std::vector;

template<class T>
class SpriteGroup
{
    private:
        vector<T*> sprites;
        int spritesSize = 0;

    public:
        std::vector<T*> getSprites();

        int size();

        SpriteGroup copy();
        void add(T* sprite);
        void remove(T* sprite);
        bool has(T* sprite);
        T* get(int index);
        void clear();

        void update(float ticks);
        void render();
};


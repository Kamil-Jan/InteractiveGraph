#include <Action/BackAction.h>

template <class T>
BackAction<T>::BackAction(T* object) : Action()
{
    this->object = object;
}

template <class T>
void BackAction<T>::execute()
{
    object->quit();
}


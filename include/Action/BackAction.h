#pragma once
#include <Action/Action.h>
#include <Mode/Mode.h>
#include <State/State.h>

template <class T>
class BackAction : public Action
{
    private:
        T* object;

    public:
        BackAction(T* object);
        void execute() override;
};

template class BackAction<Mode>;
template class BackAction<State>;


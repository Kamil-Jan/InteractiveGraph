#pragma once
#include <State/State.h>
#include <Action/Action.h>

class PushStateAction: public Action
{
    private:
        State* state;

    public:
        PushStateAction(State* state);
        void execute() override;
};


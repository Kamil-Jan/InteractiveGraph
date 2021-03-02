#include <Action/PushStateAction.h>
#include <Game.h>

PushStateAction::PushStateAction(State* state)
{
    this->state = state;
}

void PushStateAction::execute()
{
    state->resetQuitRequested();
    Game::getInstance()->push(state);
}


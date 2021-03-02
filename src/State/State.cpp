#include <State/State.h>

State::State()
{
    mQuitRequested = false;
}

void State::quit()
{
    mQuitRequested = true;
}

void State::resetQuitRequested()
{
    mQuitRequested = false;
}

bool State::quitRequested()
{
    return mQuitRequested;
}


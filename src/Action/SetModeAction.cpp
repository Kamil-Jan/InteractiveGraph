#include <State/MainState.h>
#include <Action/SetModeAction.h>

SetModeAction::SetModeAction(Mode* mode)
{
    this->mode = mode;
}

void SetModeAction::execute()
{
    if (MainState::getInstance()->getCurrentMode() != NULL)
        MainState::getInstance()->getCurrentMode()->quit();

    MainState::getInstance()->getMainMenu()->resetWidgets();
    MainState::getInstance()->setCurrentMode(mode);
}


#include <State/MainState.h>
#include <Action/ResetWidgets.h>

void ResetWidgets::execute()
{
    if (MainState::getInstance()->getCurrentMode() != NULL)
        MainState::getInstance()->getCurrentMode()->quit();
    MainState::getInstance()->getMainMenu()->resetWidgets();
}


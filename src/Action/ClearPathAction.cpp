#include <Mode/ShortestPathMode.h>
#include <Action/ClearPathAction.h>

void ClearPathAction::execute()
{
    ShortestPathMode* menu = ShortestPathMode::getInstance();
    menu->resetSelectedVertex();
    menu->resetPath();
}


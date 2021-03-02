#include <State/MainState.h>
#include <Graph/Graph.h>
#include <Action/ClearVerticesAction.h>

void ClearVerticesAction::execute()
{
    if (MainState::getInstance()->getCurrentMode())
        MainState::getInstance()->getCurrentMode()->quit();
    Graph::getInstance()->clear();
}


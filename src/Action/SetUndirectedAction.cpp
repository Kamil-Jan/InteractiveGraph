#include <Action/SetUndirectedAction.h>
#include <State/MainState.h>
#include <Graph/Graph.h>

void SetUndirectedAction::execute()
{
    MainState::getInstance()->getBottomMenu()->resetWidgets();
    Graph::getInstance()->setUndirected();
}


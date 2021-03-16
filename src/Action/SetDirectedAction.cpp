#include <Action/SetDirectedAction.h>
#include <State/MainState.h>
#include <Graph/Graph.h>

void SetDirectedAction::execute()
{
    MainState::getInstance()->getBottomMenu()->resetWidgets();
    Graph::getInstance()->setDirected();
}


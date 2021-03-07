#include <Mode/ArticulationPointsMode.h>
#include <Graph/Graph.h>
#include <Logger.h>

void ArticulationPointsMode::update(float ticks)
{
    Logger::debug("ArticulationPointsMode::update");
    if (artPoints.empty() && Graph::getInstance()->getVerticesCount() != 0)
        artPoints = Graph::getInstance()->findArticulationPoints();

    for (Vertex* artPoint : artPoints)
        artPoint->setSelected();
}

void ArticulationPointsMode::quit()
{
    artPoints.clear();
}

void ArticulationPointsMode::render() {}


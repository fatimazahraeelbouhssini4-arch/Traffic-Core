#include "RoadSegment.h"

void RoadSegment::setGeometry(std::unique_ptr<RoadGeometryStrategy> s)
{
    strategy = std::move(s);
}

void RoadSegment::draw() const
{
    if (strategy)
        strategy->draw();
}

#pragma once
#include <memory>
#include "geometry/RoadGeometryStrategy.h"

class RoadSegment {
private:
    std::unique_ptr<RoadGeometryStrategy> strategy;

public:
    void setGeometry(std::unique_ptr<RoadGeometryStrategy> s);
    void draw() const;
};

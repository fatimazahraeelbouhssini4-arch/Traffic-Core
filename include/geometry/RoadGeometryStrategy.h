#pragma once
#include <raylib.h>

class RoadGeometryStrategy {
public:
    virtual ~RoadGeometryStrategy() = default;
    virtual void draw() const = 0;
};

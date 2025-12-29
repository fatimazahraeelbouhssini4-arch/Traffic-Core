#pragma once
#include <vector>
#include <memory>
#include "RoadSegment.h"
#include <raylib.h>

class Intersection {
private:
    Vector3 position;
    std::vector<std::shared_ptr<RoadSegment>> roads;

public:
    Intersection(const Vector3& pos);
    void addRoad(std::shared_ptr<RoadSegment> road);
    void draw() const;
    Vector3 getPosition() const;
};

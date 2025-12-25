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
    Intersection(const Vector3& pos) : position(pos) {}

    void addRoad(std::shared_ptr<RoadSegment> road) {
        roads.push_back(road);
    }

    void draw() const {
        for (const auto& road : roads) {
            if (road)
                road->draw();
        }
    }

    Vector3 getPosition() const { return position; }
};

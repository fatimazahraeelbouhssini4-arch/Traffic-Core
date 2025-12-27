#include "Intersection.h"

Intersection::Intersection(const Vector3& pos) : position(pos) {}

void Intersection::addRoad(std::shared_ptr<RoadSegment> road) {
    roads.push_back(road);
}

void Intersection::draw() const {
    for (auto& road : roads) {
        if (road) road->draw();
    }
  
}

Vector3 Intersection::getPosition() const {
    return position;
}

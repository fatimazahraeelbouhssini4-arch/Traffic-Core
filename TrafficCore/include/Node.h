#ifndef NODE_H
#define NODE_H

#include <raylib.h>
#include <vector>
#include <memory>

class RoadSegment;

class Node
{
private:
    Vector3 position;
    std::vector<std::shared_ptr<RoadSegment>> roads;

public:
    Node(const Vector3& pos);

    void addRoad(const std::shared_ptr<RoadSegment>& road);
    Vector3 getPosition() const;

    void draw() const;
};

#endif

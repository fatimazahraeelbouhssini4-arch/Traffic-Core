#include "Node.h"
#include "RoadSegment.h"

Node::Node(const Vector3& pos)
    : position(pos) {}

void Node::addRoad(const std::shared_ptr<RoadSegment>& road)
{
    roads.push_back(road);
}

Vector3 Node::getPosition() const
{
    return position;
}

void Node::draw() const
{
  DrawSphere(position, 0.8f, RED);
    // Dessiner les routes connectées
    for (const auto& road : roads)
    {
        if (road)
            road->draw();
    }
}

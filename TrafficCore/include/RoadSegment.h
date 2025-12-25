#ifndef ROADSEGMENT_H
#define ROADSEGMENT_H

#include "Node.h"
#include "geometry/RoadGeometryStrategy.h"
#include <memory>
#include <vector>

class RoadSegment {
private:
    Node* startNode;
    Node* endNode;
    int lanes;
    float laneWidth;
    std::unique_ptr<RoadGeometryStrategy> geometry;
    
    // Trottoirs
    struct Sidewalk {
        std::vector<Vector3> path;
        float width;
        float height;
    };
    std::vector<Sidewalk> sidewalks;
    
    void CreateGeometry(bool useCurvedConnection);
    void CreateSidewalks();
    void DrawSidewalk(const Sidewalk& sidewalk) const;
    
public:
    RoadSegment(Node* start, Node* end, int lanes, bool useCurvedConnection = true);
    
    void Draw() const;
    
    // Getters
    Node* GetStartNode() const { return startNode; }
    Node* GetEndNode() const { return endNode; }
    int GetLanes() const { return lanes; }
    float GetWidth() const { return lanes * laneWidth; }
    float GetLength() const;
    RoadGeometryStrategy* GetGeometry() const { return geometry.get(); }
};

#endif
#ifndef ROADSEGMENT_H
#define ROADSEGMENT_H

#include "Node.h"
#include "raymath.h"

class RoadSegment {
private:
    Node* startNode;  // Pointeur vers le noeud de départ
    Node* endNode;    // Pointeur vers le noeud d'arrivée
    int lanes;
    float laneWidth;

public:
    RoadSegment(Node* start, Node* end, int lanes);

    void Draw() const;
    
    // Getters pour les calculs de trajectoire des voitures
    Node* GetStartNode() const { return startNode; }
    Node* GetEndNode() const { return endNode; }
    float GetLength() const;
};

#endif
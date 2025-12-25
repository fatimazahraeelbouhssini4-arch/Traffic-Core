#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Node.h"
#include "geometry/RoundaboutGeometry.h"
#include <memory>

class Intersection {
private:
    Node* node;
    std::unique_ptr<RoundaboutGeometry> roundaboutGeometry;
    
public:
    Intersection(Node* node);
    
    void Draw() const;
    Node* GetNode() const { return node; }
    
    // Méthodes pour la gestion du trafic (à implémenter plus tard)
    bool CanEnter(class Vehicle* vehicle) const;
    void Update(float deltaTime);
};

#endif
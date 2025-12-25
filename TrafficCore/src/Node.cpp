#include "Node.h"

Node::Node(int id, Vector3 pos, NodeType type, float radius) 
    : id(id), position(pos), type(type), radius(radius) {}

void Node::Draw() const {
    Color roadColor = { 50, 50, 50, 255 };
    
    if (type == ROUNDABOUT) {
        // Dessin du bitume du rond-point
        DrawCylinder(position, radius, radius, 0.2f, 32, roadColor);
        // L'herbe au centre (Smart Boukhalf)
        DrawCylinder(position, radius * 0.5f, radius * 0.5f, 0.3f, 32, { 34, 139, 34, 255 });
    } else {
        // Intersection simple (pour cacher les angles des routes)
        DrawCylinder(position, radius, radius, 0.1f, 24, roadColor);
    }
}
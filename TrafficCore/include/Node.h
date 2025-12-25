#ifndef NODE_H
#define NODE_H

#include "raylib.h"

enum NodeType { SIMPLE_INTERSECTION, ROUNDABOUT, TRAFFIC_LIGHT };

class Node {
private:
    int id;
    Vector3 position;
    NodeType type;
    float radius;

public:
    Node(int id, Vector3 pos, NodeType type = SIMPLE_INTERSECTION, float radius = 5.0f);
    
    // Getters
    Vector3 GetPosition() const { return position; }
    int GetId() const { return id; }
    NodeType GetType() const { return type; }
    float GetRadius() const { return radius; }

    // Setters (utile si on veut déplacer le stade ou l'université)
    void SetPosition(Vector3 pos) { position = pos; }

    void Draw() const;
};

#endif
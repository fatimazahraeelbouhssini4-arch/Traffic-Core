#include "Node.h"
#include "RoadSegment.h"
#include "raymath.h"

Node::Node(int id, Vector3 position, NodeType type, float radius)
    : id(id), position(position), type(type), radius(radius) {}

void Node::AddConnectedRoad(RoadSegment* road) {
    connectedRoads.push_back(road);
}

Vector3 Node::GetConnectionTangent(Vector3 direction) const {
    direction = Vector3Normalize(direction);
    
    if (type == ROUNDABOUT) {
        // Pour un rond-point, la tangente est perpendiculaire au rayon
        Vector3 toCenter = Vector3Subtract(position, Vector3Add(position, Vector3Scale(direction, radius)));
        return Vector3CrossProduct(toCenter, {0, 1, 0});
    }
    
    // Pour les intersections simples, la direction reste la même
    return direction;
}

void Node::Draw() const {
    Color roadColor = {50, 50, 50, 255};
    
    switch (type) {
        case ROUNDABOUT:
            // Le rond-point est dessiné via RoundaboutGeometry dans Intersection
            break;
            
        case TRAFFIC_LIGHT:
            // Zone d'intersection carrée
            DrawCube(position, radius * 2, 0.01f, radius * 2, roadColor);
            
            // Feux tricolores aux 4 coins
            {
                Vector3 corners[4] = {
                    {position.x + radius, position.y, position.z + radius},
                    {position.x + radius, position.y, position.z - radius},
                    {position.x - radius, position.y, position.z + radius},
                    {position.x - radius, position.y, position.z - radius}
                };
                
                for (int i = 0; i < 4; i++) {
                    // Poteau
                    DrawCylinder(corners[i], 0.15f, 0.15f, 4.0f, 8, DARKGRAY);
                    
                    // Boîtier du feu
                    Vector3 lightPos = {corners[i].x, corners[i].y + 4.0f, corners[i].z};
                    DrawCube(lightPos, 0.5f, 1.2f, 0.3f, BLACK);
                    
                    // Feux (rouge, jaune, vert)
                    DrawSphere({lightPos.x, lightPos.y + 0.4f, lightPos.z + 0.2f}, 0.15f, RED);
                    DrawSphere({lightPos.x, lightPos.y, lightPos.z + 0.2f}, 0.15f, YELLOW);
                    DrawSphere({lightPos.x, lightPos.y - 0.4f, lightPos.z + 0.2f}, 0.15f, GREEN);
                }
            }
            break;
            
        case SIMPLE_INTERSECTION:
        default:
            // Zone circulaire simple
            DrawCylinder(position, radius, radius, 0.01f, 32, roadColor);
            break;
    }
}
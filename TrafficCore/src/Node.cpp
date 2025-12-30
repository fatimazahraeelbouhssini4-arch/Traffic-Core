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
    Color lineColor = {255, 255, 255, 255};
    
    switch (type) {
        case ROUNDABOUT:
            // Le rond-point est dessiné via RoundaboutGeometry dans Intersection
            break;
            
        case TRAFFIC_LIGHT: {
            // Zone d'intersection carrée
            float size = radius * 1.5f;
            DrawCube(position, size * 2, 0.01f, size * 2, roadColor);
            
            // Passages piétons (4 côtés)
            float stripWidth = 0.8f;
            float stripLength = 3.0f;
            float stripSpacing = 1.0f;
            int numStrips = 4;
            
            // Passage Nord
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x + offset, position.y + 0.02f, position.z - size - 1.5f}, 
                        stripWidth, 0.01f, stripLength, lineColor);
            }
            // Passage Sud
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x + offset, position.y + 0.02f, position.z + size + 1.5f}, 
                        stripWidth, 0.01f, stripLength, lineColor);
            }
            // Passage Est
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x + size + 1.5f, position.y + 0.02f, position.z + offset}, 
                        stripLength, 0.01f, stripWidth, lineColor);
            }
            // Passage Ouest
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x - size - 1.5f, position.y + 0.02f, position.z + offset}, 
                        stripLength, 0.01f, stripWidth, lineColor);
            }
            
            // Feux tricolores aux 4 coins
            Vector3 corners[4] = {
                {position.x + size, position.y, position.z + size},
                {position.x + size, position.y, position.z - size},
                {position.x - size, position.y, position.z + size},
                {position.x - size, position.y, position.z - size}
            };
            
            for (int i = 0; i < 4; i++) {
                // Poteau
                DrawCylinder(corners[i], 0.1f, 0.1f, 3.5f, 8, DARKGRAY);
                
                // Boîtier du feu
                Vector3 lightPos = {corners[i].x, corners[i].y + 3.5f, corners[i].z};
                DrawCube(lightPos, 0.4f, 1.0f, 0.25f, BLACK);
                
                // Feux (rouge, jaune, vert)
                DrawSphere({lightPos.x, lightPos.y + 0.35f, lightPos.z + 0.15f}, 0.12f, RED);
                DrawSphere({lightPos.x, lightPos.y, lightPos.z + 0.15f}, 0.12f, YELLOW);
                DrawSphere({lightPos.x, lightPos.y - 0.35f, lightPos.z + 0.15f}, 0.12f, GREEN);
            }
            break;
        }
        
        case SIMPLE_INTERSECTION:
        default: {
            // Intersection simple réaliste : carré gris avec passages piétons
            float intersectionSize = radius * 2.0f;
            DrawCube(position, intersectionSize, 0.01f, intersectionSize, roadColor);
            
            // Passages piétons (bandes blanches)
            float stripWidth = 0.6f;
            float stripLength = 2.5f;
            float stripSpacing = 0.8f;
            int numStrips = 5;
            
            // Passages Nord
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x + offset, position.y + 0.02f, position.z - radius - 1.2f}, 
                        stripWidth, 0.01f, stripLength, lineColor);
            }
            
            // Passages Sud
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x + offset, position.y + 0.02f, position.z + radius + 1.2f}, 
                        stripWidth, 0.01f, stripLength, lineColor);
            }
            
            // Passages Est
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x + radius + 1.2f, position.y + 0.02f, position.z + offset}, 
                        stripLength, 0.01f, stripWidth, lineColor);
            }
            
            // Passages Ouest
            for (int i = 0; i < numStrips; i++) {
                float offset = (i - numStrips/2.0f) * stripSpacing;
                DrawCube({position.x - radius - 1.2f, position.y + 0.02f, position.z + offset}, 
                        stripLength, 0.01f, stripWidth, lineColor);
            }
            
            // Lignes jaunes au centre (optionnel)
            DrawCube({position.x, position.y + 0.02f, position.z}, 
                    intersectionSize * 0.8f, 0.01f, 0.1f, {255, 200, 0, 255});
            DrawCube({position.x, position.y + 0.02f, position.z}, 
                    0.1f, 0.01f, intersectionSize * 0.8f, {255, 200, 0, 255});
            
            break;
        }
    }
}
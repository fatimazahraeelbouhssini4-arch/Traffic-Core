#include "geometry/RoundaboutGeometry.h"
#include "raymath.h"
#include "rlgl.h"
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

RoundaboutGeometry::RoundaboutGeometry(Vector3 center, float radius, float roadWidth)
    : center(center), roadWidth(roadWidth), segments(64) {
    this->outerRadius = radius;
    this->innerRadius = radius - roadWidth;
}

void RoundaboutGeometry::Draw() const {
    DrawRoadCircle();
    DrawCentralIsland();
    DrawRoadMarkings();
}

void RoundaboutGeometry::DrawRoadCircle() const {
    // Utiliser RL_TRIANGLES au lieu de RL_TRIANGLE_STRIP
    rlBegin(RL_TRIANGLES);
        rlColor4ub(50, 50, 50, 255);
        for (int i = 0; i < segments; i++) {
            float angle1 = (float)i / segments * 2 * PI;
            float angle2 = (float)(i + 1) / segments * 2 * PI;
            
            float cos1 = cosf(angle1);
            float sin1 = sinf(angle1);
            float cos2 = cosf(angle2);
            float sin2 = sinf(angle2);
            
            // Points extérieurs
            Vector3 outer1 = {
                center.x + outerRadius * cos1,
                center.y + 0.01f,
                center.z + outerRadius * sin1
            };
            Vector3 outer2 = {
                center.x + outerRadius * cos2,
                center.y + 0.01f,
                center.z + outerRadius * sin2
            };
            
            // Points intérieurs
            Vector3 inner1 = {
                center.x + innerRadius * cos1,
                center.y + 0.01f,
                center.z + innerRadius * sin1
            };
            Vector3 inner2 = {
                center.x + innerRadius * cos2,
                center.y + 0.01f,
                center.z + innerRadius * sin2
            };
            
            // Premier triangle
            rlVertex3f(outer1.x, outer1.y, outer1.z);
            rlVertex3f(inner1.x, inner1.y, inner1.z);
            rlVertex3f(outer2.x, outer2.y, outer2.z);
            
            // Deuxième triangle
            rlVertex3f(outer2.x, outer2.y, outer2.z);
            rlVertex3f(inner1.x, inner1.y, inner1.z);
            rlVertex3f(inner2.x, inner2.y, inner2.z);
        }
    rlEnd();
}

void RoundaboutGeometry::DrawCentralIsland() const {
    DrawCylinder(center, innerRadius, innerRadius, 0.3f, segments, {34, 139, 34, 255});
}

void RoundaboutGeometry::DrawRoadMarkings() const {
    // Lignes blanches extérieures
    for (int i = 0; i < segments; i += 4) {
        float angle1 = (float)i / segments * 2 * PI;
        float angle2 = (float)(i + 2) / segments * 2 * PI;
        
        Vector3 p1 = {
            center.x + outerRadius * cosf(angle1),
            center.y + 0.02f,
            center.z + outerRadius * sinf(angle1)
        };
        Vector3 p2 = {
            center.x + outerRadius * cosf(angle2),
            center.y + 0.02f,
            center.z + outerRadius * sinf(angle2)
        };
        
        DrawLine3D(p1, p2, WHITE);
    }
    
    // Lignes pointillées intérieures
    for (int i = 0; i < segments; i += 4) {
        float angle1 = (float)i / segments * 2 * PI;
        float angle2 = (float)(i + 2) / segments * 2 * PI;
        
        Vector3 p1 = {
            center.x + innerRadius * cosf(angle1),
            center.y + 0.02f,
            center.z + innerRadius * sinf(angle1)
        };
        Vector3 p2 = {
            center.x + innerRadius * cosf(angle2),
            center.y + 0.02f,
            center.z + innerRadius * sinf(angle2)
        };
        
        DrawLine3D(p1, p2, WHITE);
    }
}

std::vector<Vector3> RoundaboutGeometry::GetPoints() const {
    std::vector<Vector3> points;
    for (int i = 0; i <= segments; i++) {
        float angle = (float)i / segments * 2 * PI;
        points.push_back({
            center.x + outerRadius * cosf(angle),
            center.y,
            center.z + outerRadius * sinf(angle)
        });
    }
    return points;
}

float RoundaboutGeometry::GetLength() const {
    return 2 * PI * outerRadius;
}
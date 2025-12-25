#include "RoadSegment.h"
#include "rlgl.h"

RoadSegment::RoadSegment(Node* start, Node* end, int lanes) 
    : startNode(start), endNode(end), lanes(lanes) {
    this->laneWidth = 3.5f;
}

float RoadSegment::GetLength() const {
    return Vector3Distance(startNode->GetPosition(), endNode->GetPosition());
}

void RoadSegment::Draw() const {
    if (!startNode || !endNode) return;

    Vector3 startPos = startNode->GetPosition();
    Vector3 endPos = endNode->GetPosition();

    float distance = GetLength();
    float angle = atan2f(endPos.x - startPos.x, endPos.z - startPos.z) * RAD2DEG;
    Vector3 centerPos = Vector3Lerp(startPos, endPos, 0.5f);
    
    float totalRoadWidth = lanes * laneWidth;
    float sidewalkWidth = 1.5f;

    rlPushMatrix();
        rlTranslatef(centerPos.x, 0.01f, centerPos.z);
        rlRotatef(angle, 0, 1, 0);

        // 1. Asphalte (Couleur COULEUR_ASPHALTE)
        DrawCube({0, 0, 0}, totalRoadWidth, 0.02f, distance, {50, 50, 50, 255});

        // 2. Trottoirs (Automatique selon la largeur de la route)
        float offsetTrottoir = (totalRoadWidth / 2.0f) + (sidewalkWidth / 2.0f);
        DrawCube({offsetTrottoir, 0.1f, 0}, sidewalkWidth, 0.2f, distance, {200, 200, 200, 255});
        DrawCube({-offsetTrottoir, 0.1f, 0}, sidewalkWidth, 0.2f, distance, {200, 200, 200, 255});

        // 3. Lignes blanches (Pointillés pour multivoies)
        if (lanes > 1) {
            for (int i = 1; i < lanes; i++) {
                float lineX = -(totalRoadWidth / 2.0f) + (i * laneWidth);
                for (float z = -distance / 2.0f; z < distance / 2.0f; z += 4.0f) {
                    DrawCube({lineX, 0.04f, z}, 0.15f, 0.01f, 2.0f, WHITE);
                }
            }
        }
    rlPopMatrix();
}
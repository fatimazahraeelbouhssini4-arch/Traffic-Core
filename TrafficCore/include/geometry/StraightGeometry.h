#pragma once
#include "RoadGeometryStrategy.h"
#include <raylib.h>

// Orientation des routes
enum Orientation { HORIZONTAL, VERTICAL };

class StraightGeometry : public RoadGeometryStrategy {
    Vector3 position;
    float length, width;
    int lanes;
    Orientation orientation;  // ← stocke l’orientation
public:
    StraightGeometry(Vector3 pos, float len, float w, int laneCount, Orientation ori);

    void draw() const override;
};

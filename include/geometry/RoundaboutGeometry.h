#ifndef ROUNDABOUT_GEOMETRY_H
#define ROUNDABOUT_GEOMETRY_H

#include <raylib.h>
#include <string>
#include "RoadGeometryStrategy.h"

class RoundaboutGeometry : public RoadGeometryStrategy
{
private:
    Vector3 center;
    float radius;
    float width;
    int lanes;

    bool hasDecoration;
    Model decorationModel;

    void drawCircle3D(Vector3 centerPoint,
                      float radiusVal,
                      Color color,
                      int segments) const;

public:
    RoundaboutGeometry(Vector3 c, float r, float w, int laneCount);

    void draw() const override;

    // ✔️ UN SEUL paramètre pour les modèles .glb
    void loadDecoration(const std::string& modelPath);
};

#endif

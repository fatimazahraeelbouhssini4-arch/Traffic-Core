#include "geometry/RoundaboutGeometry.h"
#include <raylib.h>
#include <rlgl.h>
#include <cmath>
#include <string>

// ================= CONSTRUCTEUR =================
RoundaboutGeometry::RoundaboutGeometry(Vector3 c, float r, float w, int laneCount)
    : center(c),
      radius(r),
      width(w),
      lanes(laneCount),
      hasDecoration(false)
{
}

// ================= FONCTION UTILITAIRE =================
void RoundaboutGeometry::drawCircle3D(Vector3 centerPoint,
                                      float radiusVal,
                                      Color color,
                                      int segments) const
{
    float angleStep = 360.0f / segments;

    for (int i = 0; i < segments; ++i)
    {
        float a1 = angleStep * i * DEG2RAD;
        float a2 = angleStep * (i + 1) * DEG2RAD;

        Vector3 p1 = {
            centerPoint.x + radiusVal * cosf(a1),
            centerPoint.y,
            centerPoint.z + radiusVal * sinf(a1)
        };

        Vector3 p2 = {
            centerPoint.x + radiusVal * cosf(a2),
            centerPoint.y,
            centerPoint.z + radiusVal * sinf(a2)
        };

        DrawLine3D(p1, p2, color);
    }
}

// ================= DESSIN =================
void RoundaboutGeometry::draw() const
{
    const int segments = 64;
    float laneWidth = width / lanes;

    // ---------- ROUTE ----------
    for (int i = 0; i < lanes; ++i)
    {
        float innerR = radius - width + i * laneWidth;
        float outerR = innerR + laneWidth;

        DrawCylinder(
            center,
            outerR,
            innerR,
            0.2f,
            segments,
            DARKGRAY
        );
    }

    // ---------- ZONE CENTRALE ----------
    DrawCylinder(
        { center.x, center.y + 0.01f, center.z },
        radius - width,
        0.0f,
        0.4f,
        segments,
        GREEN
    );

    // ---------- LIGNES & BORDURES ----------
    rlDisableDepthTest();

    for (int i = 0; i < lanes; ++i)
    {
        float innerR = radius - width + i * laneWidth;
        float outerR = innerR + laneWidth;
        float midR = (innerR + outerR) * 0.5f;

        drawCircle3D(
            { center.x, center.y + 0.21f, center.z },
            midR,
            WHITE,
            segments
        );
    }

    drawCircle3D(
        { center.x, center.y + 0.21f, center.z },
        radius,
        YELLOW,
        segments
    );

    drawCircle3D(
        { center.x, center.y + 0.21f, center.z },
        radius - width,
        YELLOW,
        segments
    );

    rlEnableDepthTest();

    // ---------- DÉCORATION ----------
    if (hasDecoration)
    {
        DrawModel(
            decorationModel,
            { center.x, center.y + 0.4f, center.z },
            1.0f,
            WHITE
        );
    }
}

// ================= DÉCORATION =================
void RoundaboutGeometry::loadDecoration(const std::string& modelPath)
{
    decorationModel = LoadModel(modelPath.c_str());
    hasDecoration = true;
}

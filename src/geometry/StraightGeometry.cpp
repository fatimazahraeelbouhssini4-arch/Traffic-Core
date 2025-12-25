
    
#include "geometry/StraightGeometry.h"
#include <raylib.h>

StraightGeometry::StraightGeometry(
    Vector3 pos,
    float len,
    float w,
    int laneCount,
    Orientation ori
)
    : position(pos),
      length(len),
      width(w),
      lanes(laneCount),
      orientation(ori)
{
}

void StraightGeometry::draw() const
{
    // ================= ROUTE =================
    DrawCube(
        position,
        (orientation == HORIZONTAL ? length : width),
        0.2f,
        (orientation == HORIZONTAL ? width : length),
        DARKGRAY
    );

    // ================= LIGNE CENTRALE =================
    // Seulement si 2 voies ou plus
    if (lanes >= 2)
    {
        
        if (orientation == HORIZONTAL)
        {
            DrawCube(
                { position.x, position.y + 0.11f, position.z - 0.15f },
                length,
                0.02f,
                0.3f,
                YELLOW
            );


       // Ligne jaune droite du centre
        DrawCube(
            { position.x, position.y + 0.11f, position.z + 0.15f},
            length,
            0.02f,
            0.03f,
            YELLOW
        );


        }
       
    }

    // ================= LIGNES POINTILLÉES =================
    // Séparation des voies (uniquement si lanes >= 2)
    if (lanes >= 2)
    {
        float laneWidth = width / lanes;

        for (int i = 1; i < lanes; i++)
        {
float offset = -width / 2 + laneWidth * i ;


            if (orientation == HORIZONTAL)
            {
                for (float x = -length / 2; x < length / 2; x += 6.0f)
                {
                    DrawCube(
                        { position.x + x, position.y + 0.11f, position.z + offset },
                        3.0f,
                        0.02f,
                        0.15f,
                        WHITE
                    );
                }
            }
            else // VERTICAL
            {
                for (float z = -length / 2 - 40.0f ; z < length / 2 - 40.0f; z += 6.0f)
                {
                    DrawCube(
                        { position.x + offset, position.y + 0.16f, position.z + z },
                        0.4f,
                        0.03f,
                        3.0f,
                        WHITE
                    );
                }
            }
        }
    }
}

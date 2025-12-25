#include <raylib.h>
#include <raymath.h>
#include "RoadSegment.h"
#include "geometry/StraightGeometry.h"
#include "geometry/RoundaboutGeometry.h"
#include "Intersection.h"
#include <memory>
#include <cmath>
#include <string>

int main()
{
    InitWindow(1200, 800, "Traffic3D - Intersections FPS Camera");
    SetTargetFPS(60);

    // ================= CAMERA =================
    Camera3D camera{};
    camera.position = { -500.0f, 2.0f, 10.0f };
    camera.target   = { -500.0f, 2.0f, 0.0f };
    camera.up       = {0.0f, 1.0f, 0.0f};
    camera.fovy     = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector2 prevMousePos = GetMousePosition();
    SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2);
    DisableCursor();

    const float speed = 0.6f;
    const float sensitivity = 0.003f;

    // ================= CONSTANTES =================
    const float roundRadius = 50.0f;
    const float distanceAvantMain = 100.0f;
    const float sideLength = 200.0f;

    // ================= MAIN ROAD =================
    auto mainRoad = std::make_shared<RoadSegment>();
    mainRoad->setGeometry(std::make_unique<StraightGeometry>(
        Vector3{-500.0f, 0.0f, 0.0f}, 1000.0f, 70.0f, 4, HORIZONTAL
    ));

    // ================= ROUNDABOUT =================
    auto roundabout = std::make_shared<RoadSegment>();
    auto roundGeo = std::make_unique<RoundaboutGeometry>(
        Vector3{0.0f, 0.0f, 0.0f}, roundRadius, 15.0f, 1
    );
    roundGeo->loadDecoration("assets/models/fountain.glb");

    roundabout->setGeometry(std::move(roundGeo));

    // ================= SIDE ROAD =================
    auto sideRoad = std::make_shared<RoadSegment>();
    sideRoad->setGeometry(std::make_unique<StraightGeometry>(
        Vector3{-500.0f, 0.0f, -distanceAvantMain}, sideLength, 40.0f, 2, VERTICAL
    ));
// ================= NEW ROADS =================
// North road
auto northRoad = std::make_shared<RoadSegment>();
northRoad->setGeometry(std::make_unique<StraightGeometry>(
    Vector3{roundRadius+10.0f, 0.0f, 0.0f}, 1000.0f, 40.0f, 2, VERTICAL
));

    // ================= INTERSECTIONS =================
    Intersection roundaboutIntersection({0.0f,0.0f,0.0f});
    roundaboutIntersection.addRoad(mainRoad);
    roundaboutIntersection.addRoad(roundabout);
    roundaboutIntersection.addRoad(northRoad);



    Intersection tIntersection({-500.0f,0.0f,-distanceAvantMain});
    tIntersection.addRoad(mainRoad);
    tIntersection.addRoad(sideRoad);

    // ================= BOUCLE =================
    float yaw = 0.0f;
    float pitch = 0.0f;

    while (!WindowShouldClose())
    {
        // ==== MOUSE LOOK ====
        Vector2 mousePos = GetMousePosition();
        Vector2 delta = Vector2Subtract(mousePos, prevMousePos);
        prevMousePos = mousePos;

        yaw   += delta.x * sensitivity;
        pitch -= delta.y * sensitivity;
        if(pitch > 1.5f) pitch = 1.5f;
        if(pitch < -1.5f) pitch = -1.5f;

        Vector3 direction;
        direction.x = cosf(pitch) * sinf(yaw);
        direction.y = sinf(pitch);
        direction.z = cosf(pitch) * cosf(yaw);
        camera.target = Vector3Add(camera.position, direction);

        // ==== KEYBOARD MOVE ====
        Vector3 forward = {direction.x, 0.0f, direction.z};
        float len = sqrtf(forward.x*forward.x + forward.z*forward.z);
        forward.x /= len; forward.z /= len;
        Vector3 right = { forward.z, 0.0f, -forward.x };

        if (IsKeyDown(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed));
        if (IsKeyDown(KEY_S)) camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
        if (IsKeyDown(KEY_A)) camera.position = Vector3Subtract(camera.position, Vector3Scale(right, speed));
        if (IsKeyDown(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right, speed));

        // ==== DESSIN ====
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);
            DrawPlane({0,-0.1f,0},{2000,2000},GREEN);
            roundaboutIntersection.draw();
            tIntersection.draw();
        EndMode3D();
        DrawFPS(10,10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

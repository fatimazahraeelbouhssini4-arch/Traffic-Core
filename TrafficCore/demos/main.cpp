#include <raylib.h>
#include <raymath.h>
#include "RoadSegment.h"
#include "geometry/StraightGeometry.h"
#include "geometry/RoundaboutGeometry.h"
#include "Intersection.h"
#include "Node.h"
#include <memory>
#include <cmath>
#include <string>

int main()
{
    InitWindow(1200, 800, "Traffic3D - Vue aérienne");
    SetTargetFPS(60);

    // ================= CAMERA AÉRIENNE =================
    Camera3D camera{};
    camera.position = { 0.0f, 200.0f, 200.0f };  // Commence en hauteur
    camera.target   = { 0.0f, 0.0f, 0.0f };      // Centre de la ville
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const float camSpeed = 5.0f;     // vitesse de déplacement
    const float camZoomSpeed = 2.0f; // vitesse de montée/descente

    // ================= CONSTANTES =================
    const float roundRadius = 50.0f;
    const float distanceAvantMain = 100.0f;
    const float sideLength = 200.0f;

    // ================= MAIN ROAD =================
    auto mainRoad = std::make_shared<RoadSegment>();
    mainRoad->setGeometry(std::make_unique<StraightGeometry>(
        Vector3{-500.0f, 0.0f, 0.0f}, 1000.0f, 70.0f, 4, HORIZONTAL
    ));

    // ================= STUDENT HOUSE ROAD =================
    auto ROADSH = std::make_shared<RoadSegment>();
    ROADSH->setGeometry(std::make_unique<StraightGeometry>(
        Vector3{-250.0f, 0.0f, -335.0f}, 600.0f, 70.0f, 4, VERTICAL
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

    // ================= NORTH ROAD =================
    auto northRoad = std::make_shared<RoadSegment>();
    northRoad->setGeometry(std::make_unique<StraightGeometry>(
        Vector3{roundRadius+10.0f, 0.0f, 0.0f}, 1000.0f, 40.0f, 2, VERTICAL
    ));

    // ================= NODES =================
    auto mainStart0 = std::make_shared<Node>(Vector3{-997.0f, 0.0f, -30.0f});
    auto mainStart1 = std::make_shared<Node>(Vector3{-997.0f, 0.0f, -10.0f});
    auto mainStart2 = std::make_shared<Node>(Vector3{-997.0f, 0.0f, 10.0f});
    auto mainStart3 = std::make_shared<Node>(Vector3{-997.0f, 0.0f, 30.0f});
    mainStart0->addRoad(mainRoad);
    mainStart1->addRoad(mainRoad);
    mainStart2->addRoad(mainRoad);
    mainStart3->addRoad(mainRoad);

    auto mainEnd0 = std::make_shared<Node>(Vector3{-50.0f, 0.0f, -30.0f});
    auto mainEnd1 = std::make_shared<Node>(Vector3{-50.0f, 0.0f, -10.0f});
    auto mainEnd2 = std::make_shared<Node>(Vector3{-50.0f, 0.0f, 10.0f});
    auto mainEnd3 = std::make_shared<Node>(Vector3{-50.0f, 0.0f, 30.0f});
    mainEnd0->addRoad(mainRoad);
    mainEnd1->addRoad(mainRoad);
    mainEnd2->addRoad(mainRoad);
    mainEnd3->addRoad(mainRoad);

    auto sideStart1 = std::make_shared<Node>(Vector3{-510.0f, 0.0f, -35.0f});
    auto sideEnd1   = std::make_shared<Node>(Vector3{-510.0f, 0.0f, -2*distanceAvantMain});
    auto sideStart2 = std::make_shared<Node>(Vector3{-490.0f, 0.0f, -35.0f});
    auto sideEnd2   = std::make_shared<Node>(Vector3{-490.0f, 0.0f, -2*distanceAvantMain});
    sideStart1->addRoad(sideRoad);
    sideEnd1->addRoad(sideRoad);
    sideEnd1->addRoad(mainRoad);
    sideStart2->addRoad(sideRoad);
    sideEnd2->addRoad(sideRoad);
    sideEnd2->addRoad(mainRoad);

    auto northNodeH0 = std::make_shared<Node>(Vector3{ roundRadius+5.0f, 0.0f, 20.0f });
    auto northNodeH1 = std::make_shared<Node>(Vector3{ roundRadius+5.0f, 0.0f, 497.0f });
    auto northNodeH2 = std::make_shared<Node>(Vector3{ roundRadius+15.0f, 0.0f, 20.0f });
    auto northNodeH3 = std::make_shared<Node>(Vector3{ roundRadius+15.0f, 0.0f, 497.0f });
    northNodeH0->addRoad(northRoad);
    northNodeH1->addRoad(northRoad);
    northNodeH2->addRoad(northRoad);
    northNodeH3->addRoad(northRoad);

    auto roundNode0 = std::make_shared<Node>(Vector3{ 42.5f, 0.0f, 0.0f });
    auto roundNode1 = std::make_shared<Node>(Vector3{ 30.05f, 0.0f, 30.05f });
    auto roundNode2 = std::make_shared<Node>(Vector3{ 0.0f, 0.0f, 42.5f });
    auto roundNode3 = std::make_shared<Node>(Vector3{ -30.05f, 0.0f, 30.05f });
    auto roundNode4 = std::make_shared<Node>(Vector3{ -42.5f, 0.0f, 0.0f });
    auto roundNode5 = std::make_shared<Node>(Vector3{ -30.05f, 0.0f, -30.05f });
    auto roundNode6 = std::make_shared<Node>(Vector3{ 0.0f, 0.0f, -42.5f });
    auto roundNode7 = std::make_shared<Node>(Vector3{ 30.05f, 0.0f, -30.05f });
    roundNode0->addRoad(roundabout);
    roundNode1->addRoad(roundabout);
    roundNode2->addRoad(roundabout);
    roundNode3->addRoad(roundabout);
    roundNode4->addRoad(roundabout);
    roundNode5->addRoad(roundabout);
    roundNode6->addRoad(roundabout);
    roundNode7->addRoad(roundabout);

    Intersection roundaboutIntersection({0.0f,0.0f,0.0f});
    roundaboutIntersection.addRoad(mainRoad);
    roundaboutIntersection.addRoad(roundabout);
    roundaboutIntersection.addRoad(northRoad);

    // ================= BOUCLE PRINCIPALE =================
    while (!WindowShouldClose())
    {
        // ====== Déplacement caméra ======
        Vector3 forward = Vector3Subtract(camera.target, camera.position);
        forward.y = 0;
        forward = Vector3Normalize(forward);
        Vector3 right = Vector3CrossProduct(forward, camera.up);

        if (IsKeyDown(KEY_W)) {
            camera.position = Vector3Add(camera.position, Vector3Scale(forward, camSpeed));
            camera.target   = Vector3Add(camera.target, Vector3Scale(forward, camSpeed));
        }
        if (IsKeyDown(KEY_S)) {
            camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, camSpeed));
            camera.target   = Vector3Subtract(camera.target, Vector3Scale(forward, camSpeed));
        }
        if (IsKeyDown(KEY_A)) {
            camera.position = Vector3Subtract(camera.position, Vector3Scale(right, camSpeed));
            camera.target   = Vector3Subtract(camera.target, Vector3Scale(right, camSpeed));
        }
        if (IsKeyDown(KEY_D)) {
            camera.position = Vector3Add(camera.position, Vector3Scale(right, camSpeed));
            camera.target   = Vector3Add(camera.target, Vector3Scale(right, camSpeed));
        }
        if (IsKeyDown(KEY_Q)) camera.position.y += camZoomSpeed;
        if (IsKeyDown(KEY_E)) camera.position.y -= camZoomSpeed;

        // ====== DESSIN ======
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        DrawPlane({0,0,0},{2000,2000},GREEN);
        roundaboutIntersection.draw();
        mainRoad->draw();
        ROADSH->draw();
        sideRoad->draw();
        northRoad->draw();

        mainStart0->draw(); mainStart1->draw(); mainStart2->draw(); mainStart3->draw();
        mainEnd0->draw(); mainEnd1->draw(); mainEnd2->draw(); mainEnd3->draw();
        sideStart1->draw(); sideEnd1->draw(); sideStart2->draw(); sideEnd2->draw();
        roundNode0->draw(); roundNode1->draw(); roundNode2->draw(); roundNode3->draw();
        roundNode4->draw(); roundNode5->draw(); roundNode6->draw(); roundNode7->draw();
        northNodeH0->draw(); northNodeH1->draw(); northNodeH2->draw(); northNodeH3->draw();

        EndMode3D();
        DrawFPS(10,10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

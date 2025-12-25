#include "raylib.h"
#include "RoadNetwork.h"
#include <iostream>

// Fonction pour créer un réseau de test
void CreateTestNetwork(RoadNetwork& network) {
    std::cout << "Création du réseau de test..." << std::endl;

    // Créer une grille de noeuds
    Node* n1 = network.AddNode({-60, 0, -60}, ROUNDABOUT, 12.0f);
    Node* n2 = network.AddNode({0, 0, -60}, SIMPLE_INTERSECTION, 8.0f);
    Node* n3 = network.AddNode({60, 0, -60}, TRAFFIC_LIGHT, 10.0f);

    Node* n4 = network.AddNode({-60, 0, 0}, SIMPLE_INTERSECTION, 8.0f);
    Node* n5 = network.AddNode({0, 0, 0}, ROUNDABOUT, 15.0f);
    Node* n6 = network.AddNode({60, 0, 0}, SIMPLE_INTERSECTION, 8.0f);

    Node* n7 = network.AddNode({-60, 0, 60}, TRAFFIC_LIGHT, 10.0f);
    Node* n8 = network.AddNode({0, 0, 60}, SIMPLE_INTERSECTION, 8.0f);
    Node* n9 = network.AddNode({60, 0, 60}, ROUNDABOUT, 12.0f);

    // Intersections
    network.AddIntersection(n1);
    network.AddIntersection(n2);
    network.AddIntersection(n3);
    network.AddIntersection(n4);
    network.AddIntersection(n5);
    network.AddIntersection(n6);
    network.AddIntersection(n7);
    network.AddIntersection(n8);
    network.AddIntersection(n9);

    // Routes horizontales
    network.AddRoadSegment(n1, n2, 3, true);
    network.AddRoadSegment(n2, n3, 3, true);
    network.AddRoadSegment(n4, n5, 4, true);
    network.AddRoadSegment(n5, n6, 4, true);
    network.AddRoadSegment(n7, n8, 3, true);
    network.AddRoadSegment(n8, n9, 3, true);

    // Routes verticales
    network.AddRoadSegment(n1, n4, 2, true);
    network.AddRoadSegment(n4, n7, 2, true);
    network.AddRoadSegment(n2, n5, 3, true);
    network.AddRoadSegment(n5, n8, 3, true);
    network.AddRoadSegment(n3, n6, 2, true);
    network.AddRoadSegment(n6, n9, 2, true);

    network.PrintNetworkInfo();
}

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Smart Boukhalf - Test Réseau");
    SetTargetFPS(60);

    RoadNetwork network;
    CreateTestNetwork(network);

    Camera3D camera = {0};
    camera.position = {0.0f, 120.0f, 120.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    bool showHelp = true;
    bool showStats = true;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_H)) showHelp = !showHelp;
        if (IsKeyPressed(KEY_S)) showStats = !showStats;
        if (IsKeyPressed(KEY_R)) {
            camera.position = {0.0f, 120.0f, 120.0f};
            camera.target = {0.0f, 0.0f, 0.0f};
        }

        UpdateCamera(&camera, CAMERA_FREE);
        network.Update(GetFrameTime());

        BeginDrawing();
            ClearBackground(Color{135, 206, 235, 255});

            BeginMode3D(camera);
                DrawGrid(40, 10.0f);
                network.Draw();
            EndMode3D();

            DrawText("SMART BOUKHALF - Réseau Routier", 10, 10, 26, DARKGRAY);

            if (showHelp) {
                DrawRectangle(10, 60, 320, 140, Fade(BLACK, 0.7f));
                DrawText("H - Aide", 20, 70, 14, WHITE);
                DrawText("S - Stats", 20, 90, 14, WHITE);
                DrawText("R - Reset caméra", 20, 110, 14, WHITE);
                DrawText("ESC - Quitter", 20, 130, 14, WHITE);
            }

            if (showStats) {
                int y = showHelp ? 210 : 60;
                DrawRectangle(10, y, 300, 110, Fade(BLACK, 0.7f));
                DrawText(TextFormat("Noeuds: %d", network.GetNodeCount()), 20, y + 20, 14, WHITE);
                DrawText(TextFormat("Routes: %d", network.GetRoadSegmentCount()), 20, y + 40, 14, WHITE);
                DrawText(TextFormat("Intersections: %d", network.GetIntersectionCount()), 20, y + 60, 14, WHITE);
                DrawText(TextFormat("Longueur totale: %.1f", network.GetTotalRoadLength()), 20, y + 80, 14, WHITE);
            }

            DrawFPS(screenWidth - 100, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

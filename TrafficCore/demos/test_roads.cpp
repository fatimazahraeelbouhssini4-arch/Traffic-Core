#include "RoadNetwork.h"
#include "raylib.h"
#include <iostream>

// Fonction pour créer un petit réseau de test
void CreateTestNetwork(RoadNetwork& network) {
    // Ajouter des noeuds
    Node* n1 = network.AddNode({-20.0f, 0.0f, 0.0f});
    Node* n2 = network.AddNode({20.0f, 0.0f, 0.0f});
    Node* n3 = network.AddNode({0.0f, 0.0f, 0.0f}, ROUNDABOUT, 10.0f);
    Node* n4 = network.AddNode({0.0f, 0.0f, -20.0f});
    Node* n5 = network.AddNode({0.0f, 0.0f, 20.0f});

    // Ajouter des segments de route
    network.AddRoadSegment(n1, n3, 2);
    network.AddRoadSegment(n2, n3, 2);
    network.AddRoadSegment(n4, n3, 2);
    network.AddRoadSegment(n5, n3, 2);

    // Ajouter les intersections
    network.AddIntersection(n3);
}

int main() {
    // Initialiser la fenêtre Raylib
    InitWindow(800, 600, "Test Réseau Routier");
    SetTargetFPS(60);

    // Initialiser la caméra 3D
    Camera3D camera;
    camera.position = {0.0f, 30.0f, 60.0f};   // Position de la caméra
    camera.target = {0.0f, 0.0f, 0.0f};       // Point que regarde la caméra
    camera.up = {0.0f, 1.0f, 0.0f};           // Vecteur "up"
    camera.fovy = 45.0f;                      // Angle de vue
    camera.projection = CAMERA_PERSPECTIVE;   // Projection perspective

    // Créer le réseau routier
    RoadNetwork network;
    CreateTestNetwork(network);

    // Boucle principale
    while (!WindowShouldClose()) {
        // Début du dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // Dessiner le réseau
        network.Draw();

        EndMode3D();

        // Afficher les infos simples
        DrawText("Test du réseau routier", 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

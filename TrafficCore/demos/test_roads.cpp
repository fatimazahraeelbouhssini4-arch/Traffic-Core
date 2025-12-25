#include "raylib.h"
#include "Node.h"
#include "RoadSegment.h"
#include <vector>

int main() {
    // Initialisation de la fenêtre
    InitWindow(1200, 800, "Test Architecture - Smart Boukhalf");

    // 1. Création des Noeuds (Points clés)
    Node* n1 = new Node(1, { -50, 0, 0 }, ROUNDABOUT, 10.0f);
    Node* n2 = new Node(2, { 50, 0, 0 }, SIMPLE_INTERSECTION, 6.0f);
    Node* n3 = new Node(3, { 0, 0, 50 }, SIMPLE_INTERSECTION, 6.0f);

    // 2. Création des Segments de route (Liaisons)
    std::vector<RoadSegment*> roads;
    roads.push_back(new RoadSegment(n1, n2, 4)); // Route à 4 voies
    roads.push_back(new RoadSegment(n1, n3, 2)); // Route à 2 voies

    // Caméra
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 60.0f, 60.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(camera);
                DrawGrid(20, 10.0f);

                // Dessiner les routes
                for (auto r : roads) r->Draw();
                
                // Dessiner les noeuds (par dessus pour cacher les raccords)
                n1->Draw();
                n2->Draw();
                n3->Draw();

            EndMode3D();
            DrawText("TEST DES ROUTES - ZSQD pour bouger la camera", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Nettoyage
    for (auto r : roads) delete r;
    delete n1; delete n2; delete n3;

    CloseWindow();
    return 0;
}

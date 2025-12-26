#include "raylib.h"
#include "RoadNetwork.h"
#include <iostream>

// Fonction pour créer un réseau de test simple
void CreateSimpleNetwork(RoadNetwork& network) {
    std::cout << "=== Création du réseau de test ===" << std::endl;
    
    // Créer quelques noeuds
    Node* n1 = network.AddNode({-50, 0, 0}, ROUNDABOUT, 12.0f);
    Node* n2 = network.AddNode({50, 0, 0}, SIMPLE_INTERSECTION, 8.0f);
    Node* n3 = network.AddNode({0, 0, 50}, TRAFFIC_LIGHT, 10.0f);
    Node* n4 = network.AddNode({50, 0, 50}, ROUNDABOUT, 10.0f);
    
    std::cout << "Noeuds créés: 4" << std::endl;
    
    // Créer les intersections
    network.AddIntersection(n1);
    network.AddIntersection(n2);
    network.AddIntersection(n3);
    network.AddIntersection(n4);
    
    std::cout << "Intersections créées: 4" << std::endl;
    
    // Créer les routes
    network.AddRoadSegment(n1, n2, 4, true);  // Route à 4 voies avec courbe
    network.AddRoadSegment(n1, n3, 2, true);  // Route à 2 voies avec courbe
    network.AddRoadSegment(n2, n4, 3, true);  // Route à 3 voies avec courbe
    network.AddRoadSegment(n3, n4, 2, true);  // Route à 2 voies avec courbe
    
    std::cout << "Routes créées: 4" << std::endl;
    
    // Afficher les informations du réseau
    network.PrintNetworkInfo();
}

int main() {
    // Initialisation
    const int screenWidth = 1400;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Smart Boukhalf - Test Routes");
    
    std::cout << "\n=== SMART BOUKHALF - Test du système routier ===" << std::endl;
    std::cout << "Initialisation de la fenêtre: " << screenWidth << "x" << screenHeight << std::endl;
    
    // Créer le réseau routier
    RoadNetwork network;
    CreateSimpleNetwork(network);
    
    // Configuration de la caméra
    Camera3D camera = { 0 };
    camera.position = Vector3{0.0f, 80.0f, 80.0f};
    camera.target = Vector3{0.0f, 0.0f, 0.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Variables pour l'interface
    bool showHelp = true;
    bool showStats = true;
    
    SetTargetFPS(60);
    
    std::cout << "Démarrage de la boucle principale..." << std::endl;
    
    // Boucle principale
    while (!WindowShouldClose()) {
        // Gestion de l'input
        if (IsKeyPressed(KEY_H)) showHelp = !showHelp;
        if (IsKeyPressed(KEY_S)) showStats = !showStats;
        if (IsKeyPressed(KEY_R)) {
            // Réinitialiser la caméra
            camera.position = Vector3{0.0f, 80.0f, 80.0f};
            camera.target = Vector3{0.0f, 0.0f, 0.0f};
        }
        
        // Contrôle de la caméra
        UpdateCamera(&camera, CAMERA_FREE);
        
        // Rendu
        BeginDrawing();
            ClearBackground(Color{135, 206, 235, 255}); // Sky blue
            
            BeginMode3D(camera);
                // Grille au sol
                DrawGrid(30, 10.0f);
                
                // Dessiner le réseau routier
                network.Draw();
                
            EndMode3D();
            
            // === Interface utilisateur ===
            
            // Titre principal
            DrawText("SMART BOUKHALF", 10, 10, 32, DARKGRAY);
            DrawText("Test du Système Routier", 10, 50, 20, DARKGREEN);
            
            // Aide
            if (showHelp) {
                DrawRectangle(10, 90, 380, 160, Fade(BLACK, 0.7f));
                DrawText("CONTRÔLES:", 20, 100, 18, YELLOW);
                DrawText("ZSQD - Déplacer la caméra", 25, 125, 14, WHITE);
                DrawText("Molette souris - Zoom", 25, 145, 14, WHITE);
                DrawText("Clic droit + Souris - Rotation", 25, 165, 14, WHITE);
                DrawText("R - Réinitialiser caméra", 25, 185, 14, WHITE);
                DrawText("H - Masquer/Afficher aide", 25, 205, 14, WHITE);
                DrawText("S - Masquer/Afficher statistiques", 25, 225, 14, WHITE);
            }
            
            // Statistiques
            if (showStats) {
                int statsY = showHelp ? 260 : 90;
                DrawRectangle(10, statsY, 320, 150, Fade(BLACK, 0.7f));
                DrawText("STATISTIQUES DU RÉSEAU:", 20, statsY + 10, 16, YELLOW);
                DrawText(TextFormat("Noeuds: %d", network.GetNodeCount()), 
                         25, statsY + 35, 14, WHITE);
                DrawText(TextFormat("Routes: %d", network.GetRoadSegmentCount()), 
                         25, statsY + 55, 14, WHITE);
                DrawText(TextFormat("Intersections: %d", network.GetIntersectionCount()), 
                         25, statsY + 75, 14, WHITE);
                DrawText(TextFormat("Longueur totale: %.1f unités", network.GetTotalRoadLength()), 
                         25, statsY + 95, 14, WHITE);
                DrawText("Design Pattern: Strategy (Géométries)", 
                         25, statsY + 120, 14, LIME);
            }
            
            // FPS et version
            DrawFPS(screenWidth - 100, 10);
            DrawText("v1.0 - Test Routes", screenWidth - 180, 40, 12, GRAY);
            
        EndDrawing();
    }
    
    std::cout << "\nFermeture de l'application..." << std::endl;
    CloseWindow();
    
    std::cout << "Application terminée avec succès !" << std::endl;
    return 0;
}
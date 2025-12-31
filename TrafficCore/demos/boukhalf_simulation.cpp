#include <utility>
#include "raylib.h"
#include "raymath.h"
#include "Vehicules/CarFactory.h"
#include "Vehicules/TrafficManager.h"
#include "Vehicules/Car.h"
#include "RoadNetwork.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <filesystem>
#include "Vehicules/ModelManager.h"
#include <cfloat>

// ==================== CAMERA ====================
Camera3D g_camera;

// ==================== UTILS ====================
void UpdateCameraOrbital(Camera3D* camera, float* distance, Vector2* angles,
                         float zoomSpeed, float rotationSpeed, float panSpeed);
void UpdateCameraFree(Camera3D* camera, float* yaw, float* pitch, 
                      float moveSpeed, float sprintSpeed, float mouseSensitivity);
void DrawUIPanel(int x, int y, int width, int height, const char* title);
void DrawEnvironment(float gridSize);

// ==================== STRING → ENUM ====================
CarModel StringToCarModel(const std::string& name) {
    if (name == "Dodge Challenger") return CarModel::DODGE_CHALLENGER;
    if (name == "Chevrolet Camaro") return CarModel::CHEVROLET_CAMARO;
    if (name == "Nissan GTR") return CarModel::NISSAN_GTR;
    if (name == "SUV") return CarModel::SUV_MODEL;
    if (name == "Taxi") return CarModel::TAXI;
    return CarModel::DODGE_CHALLENGER;
}

// ==================== ROAD NETWORK SETUP ====================
void CreateTestNetwork(RoadNetwork& network) {
    Node* n1 = network.AddNode({1000.0f, 0.0f, -450.0f}, SIMPLE_INTERSECTION, 20.0f);
    Node* n2 = network.AddNode({700.0f, 0.0f, -450.0f}, SIMPLE_INTERSECTION, 20.0f);
    Node* n3 = network.AddNode({700.0f, 0.0f, -250.0f}, SIMPLE_INTERSECTION, 8.0f);
    Node* n4 = network.AddNode({350.0f, 0.0f, -450.0f}, SIMPLE_INTERSECTION, 20.0f);
    Node* n5 = network.AddNode({350.0f, 0.1f, 0.0f}, ROUNDABOUT, 60.0f);
    Node* n6 = network.AddNode({0.0f, 0.1f, -450.0f}, ROUNDABOUT, 60.0f);
    Node* n7 = network.AddNode({0.0f, 0.0f, -600.0f}, SIMPLE_INTERSECTION, 20.0f);
    Node* n8 = network.AddNode({0.0f, 0.0f, 0.0f}, TRAFFIC_LIGHT, 25.0f);
    Node* n9 = network.AddNode({0.0f, 0.0f, 150.0f}, SIMPLE_INTERSECTION, 20.0f);
    Node* n10 = network.AddNode({-150.0f, 0.0f, 0.0f}, SIMPLE_INTERSECTION, 20.0f);
    
    network.AddRoadSegment(n1, n2, 4, false);
    network.AddRoadSegment(n2, n3, 2, false);
    network.AddRoadSegment(n2, n4, 4, false);
    network.AddRoadSegment(n4, n5, 4, false);
    network.AddRoadSegment(n4, n6, 4, false);
    network.AddRoadSegment(n7, n6, 4, false);
    network.AddRoadSegment(n6, n8, 4, false);
    network.AddRoadSegment(n8, n9, 4, false);
    network.AddRoadSegment(n8, n10, 4, false);
    network.AddRoadSegment(n5, n8, 4, false);
    
    network.AddIntersection(n1);
    network.AddIntersection(n2);
    network.AddIntersection(n3);
    network.AddIntersection(n4);
    network.AddIntersection(n5);
    network.AddIntersection(n6);
    network.AddIntersection(n7);
    network.AddIntersection(n8);
    network.AddIntersection(n9);
    network.AddIntersection(n10);
}

// ==================== FONCTION POUR DESSINER L'ENVIRONNEMENT ====================
void DrawEnvironment(float gridSize) {
    // SOL avec meilleure couleur (gazon gris-vert réaliste)
    Color groundColor = {76, 110, 75, 255};   // Vert gazon naturel
    DrawPlane({0, -0.01f, 0}, {gridSize, gridSize}, groundColor);
    
    // Lignes de grille légères pour mieux voir la perspective
    Color gridColor = {100, 130, 100, 255};  // Vert plus clair pour la grille
    int gridLines = 12;
    float gridSpacing = gridSize / gridLines;
    
    for (int i = -gridLines/2; i <= gridLines/2; i++) {
        float pos = i * gridSpacing;
        // Lignes X
        DrawLine3D({pos, 0, -gridSize/2}, {pos, 0, gridSize/2}, gridColor);
        // Lignes Z
        DrawLine3D({-gridSize/2, 0, pos}, {gridSize/2, 0, pos}, gridColor);
    }
    
    // CIEL (grand cube autour)
    Color skyColor = {173, 216, 230, 150};   // Bleu ciel léger
    DrawCube({0, gridSize/2 + 100, 0}, gridSize * 2, gridSize, gridSize * 2, skyColor);
}

// ==================== MAIN ====================
int main() {

    InitWindow(1280, 720, "SMART CITY - Traffic Core Simulator with Road Network");
    SetTargetFPS(60);

    // ==================== CAMERA INITIALIZATION ====================
    g_camera.position = {200.0f, 200.0f, 200.0f};
    g_camera.target = {0.0f, 0.0f, 0.0f};
    g_camera.up = {0.0f, 1.0f, 0.0f};
    g_camera.fovy = 60.0f;
    g_camera.projection = CAMERA_PERSPECTIVE;

    // Orbital camera parameters
    Vector2 cameraAngles = {45.0f * DEG2RAD, 35.0f * DEG2RAD};
    float cameraDistance = 250.0f;
    Vector3 cameraFocus = {0.0f, 0.0f, 0.0f};

    // Keyboard camera controls parameters
    float cameraPanSpeed = 100.0f;
    float cameraZoomSpeed = 3.0f;
    float cameraRotationSpeed = 0.05f;

    // ==================== TRAFFIC MANAGER ====================
    TrafficManager trafficMgr;

    // ==================== ROAD NETWORK ====================
    RoadNetwork network;
    CreateTestNetwork(network);

    trafficMgr.setRoadNetwork(&network);

    // ==================== CALCUL OPTIMAL DU CENTRE RÉSEAU ====================
    Vector3 networkCenter = {0, 0, 0};
    Vector3 networkMin = {FLT_MAX, FLT_MAX, FLT_MAX};
    Vector3 networkMax = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    int nodeCount = 0;

    for (const auto& nPtr : network.GetNodes()) {
        Vector3 pos = nPtr->GetPosition();
        networkCenter = Vector3Add(networkCenter, pos);
        
        // Calculer les limites
        networkMin.x = fminf(networkMin.x, pos.x);
        networkMin.y = fminf(networkMin.y, pos.y);
        networkMin.z = fminf(networkMin.z, pos.z);
        
        networkMax.x = fmaxf(networkMax.x, pos.x);
        networkMax.y = fmaxf(networkMax.y, pos.y);
        networkMax.z = fmaxf(networkMax.z, pos.z);
        
        nodeCount++;
    }

    if (nodeCount > 0) {
        networkCenter = Vector3Scale(networkCenter, 1.0f / (float)nodeCount);
        
        // Calculer la distance optimale en fonction de la taille du réseau
        Vector3 networkSize = Vector3Subtract(networkMax, networkMin);
        float networkDiagonal = Vector3Length(networkSize);
        float optimalDistance = networkDiagonal * 1.2f;
        
        cameraDistance = Clamp(optimalDistance, 150.0f, 800.0f);
        
        // Positionner la caméra directement au-dessus du réseau (vue de dessus)
        float height = networkDiagonal * 0.6f;
        g_camera.position = {
            networkCenter.x,
            networkCenter.y + height,
            networkCenter.z
        };
        g_camera.target = networkCenter;
        cameraFocus = networkCenter;

        // Ajuster les angles pour une vue de haut
        cameraAngles.x = 0.0f;
        cameraAngles.y = 0.0f;
    }

    enum CameraControlMode { ORBITAL, FREE, FPS, TOPDOWN, SIMPLE_FPS };
    CameraControlMode cameraMode = FPS;

    // FPS camera state
    float yaw = 0.0f;
    float pitch = 0.0f;
    float mouseSensitivity = 0.15f;
    float moveSpeed = 6.0f;
    float sprintSpeed = 12.0f;
    bool fpsActive = (cameraMode == FPS);
    bool simpleFpsActive = (cameraMode == SIMPLE_FPS);
    bool walkMode = false;
    
    // Top-down mode parameters
    float topdownHeight = 300.0f;
    float topdownMinHeight = 30.0f;
    float topdownMaxHeight = 1000.0f;
    float topdownPanSpeed = 200.0f;

    // ==================== MODELS ====================
    std::vector<std::string> availableModels = {
        "Dodge Challenger",
        "Chevrolet Camaro",
        "Nissan GTR",
        "SUV",
        "Taxi"
    };

    int selectedModelIndex = 0;
    int nextCarId = 1;

    // Load all .glb models from assets/models into ModelManager
    ModelManager& mm = ModelManager::getInstance();
    std::vector<std::string> candidateDirs = {
        "assets/models",
        "../TrafficCore/assets/models",
        "TrafficCore/assets/models",
        "../TrafficCore/assets/models/"
    };

    for (const auto& dir : candidateDirs) {
        try {
            if (!std::filesystem::exists(dir)) continue;
            for (const auto& entry : std::filesystem::directory_iterator(dir)) {
                if (!entry.is_regular_file()) continue;
                auto path = entry.path();
                if (path.has_extension() && (path.extension() == ".glb" || path.extension() == ".GLB")) {
                    std::string filename = path.filename().string();
                    std::string lowercase = filename;
                    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

                    std::string category = "CAR";
                    if (lowercase.find("truck") != std::string::npos) category = "TRUCK";
                    else if (lowercase.find("bus") != std::string::npos) category = "BUS";
                    else if (lowercase.find("taxi") != std::string::npos) category = "CAR";

                    mm.loadModel(category, path.string());
                }
            }
            break;
        } catch (...) {
            // ignore filesystem errors and try next
        }
    }

    // ==================== CREATE VEHICLES WITH MULTI-SEGMENT ITINERAIRES ===
    int sampleCountPerSegment = 12;
    const auto& nodes = network.GetNodes();
    int desiredVehicles = (int)network.GetRoadSegmentCount();

    for (int v = 0; v < desiredVehicles; ++v) {
        if (nodes.empty()) break;
        Node* start = nodes[GetRandomValue(0, (int)nodes.size()-1)].get();
        Node* end = start;
        int tries = 0;
        while (end == start && tries < 10) {
            end = nodes[GetRandomValue(0, (int)nodes.size()-1)].get();
            tries++;
        }

        auto nodePath = network.FindPath(start, end);
        if (nodePath.size() < 2) continue;

        std::vector<Vector3> pathPoints;

        for (size_t i = 0; i < nodePath.size() - 1; ++i) {
            Node* a = nodePath[i];
            Node* b = nodePath[i+1];
            RoadSegment* connecting = nullptr;
            for (const auto& segPtr : network.GetRoadSegments()) {
                RoadSegment* s = segPtr.get();
                if (!s) continue;
                if ((s->GetStartNode() == a && s->GetEndNode() == b) ||
                    (s->GetStartNode() == b && s->GetEndNode() == a)) {
                    connecting = s;
                    break;
                }
            }
            if (!connecting) continue;

            int laneIdx = connecting->GetLanes() > 0 ? connecting->GetLanes()/2 : 0;
            for (int s = 0; s <= sampleCountPerSegment; ++s) {
                float t = s / (float)sampleCountPerSegment;
                pathPoints.push_back(connecting->GetLanePosition(laneIdx, t));
            }
        }

        if (pathPoints.empty()) continue;

        auto car = CarFactory::createCar(StringToCarModel(availableModels[selectedModelIndex]), pathPoints.front(), nextCarId++);

        if (pathPoints.size() >= 2) {
            float dist = Vector3Distance(pathPoints[0], pathPoints[1]);
            float desiredWidth = fmaxf(1.2f, dist * 0.6f);
            float scale = desiredWidth / 2.0f;
            car->setScale(scale);
        }

        car->setItinerary(pathPoints);
        trafficMgr.addVehicle(std::move(car));
    }

    bool paused = true;
    float simTime = 0.0f;

    // ==================== LOOP ====================
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        if (!paused) simTime += dt;

        // ==================== CAMERA CONTROL ====================
        if (cameraMode == ORBITAL) {
            UpdateCameraOrbital(&g_camera, &cameraDistance, &cameraAngles,
                                2.0f, 0.005f, 0.5f);

            Vector3 forward = Vector3Normalize(Vector3Subtract(g_camera.target, g_camera.position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, g_camera.up));
            float panAmount = cameraPanSpeed * dt;

            if (IsKeyDown(KEY_Z) || IsKeyDown(KEY_W)) {
                cameraFocus = Vector3Add(cameraFocus, Vector3Scale(forward, panAmount));
                g_camera.target = Vector3Add(g_camera.target, Vector3Scale(forward, panAmount));
                g_camera.position = Vector3Add(g_camera.position, Vector3Scale(forward, panAmount));
            }
            if (IsKeyDown(KEY_S)) {
                cameraFocus = Vector3Subtract(cameraFocus, Vector3Scale(forward, panAmount));
                g_camera.target = Vector3Subtract(g_camera.target, Vector3Scale(forward, panAmount));
                g_camera.position = Vector3Subtract(g_camera.position, Vector3Scale(forward, panAmount));
            }
            if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_A)) {
                cameraFocus = Vector3Subtract(cameraFocus, Vector3Scale(right, panAmount));
                g_camera.target = Vector3Subtract(g_camera.target, Vector3Scale(right, panAmount));
                g_camera.position = Vector3Subtract(g_camera.position, Vector3Scale(right, panAmount));
            }
            if (IsKeyDown(KEY_D)) {
                cameraFocus = Vector3Add(cameraFocus, Vector3Scale(right, panAmount));
                g_camera.target = Vector3Add(g_camera.target, Vector3Scale(right, panAmount));
                g_camera.position = Vector3Add(g_camera.position, Vector3Scale(right, panAmount));
            }

            if (IsKeyDown(KEY_E)) {
                cameraDistance = Clamp(cameraDistance + cameraZoomSpeed * dt * 50.0f, 50.0f, 600.0f);
            }
            if (IsKeyDown(KEY_UP)) {
                cameraDistance = Clamp(cameraDistance + cameraZoomSpeed * dt * 50.0f, 50.0f, 600.0f);
            }
            if (IsKeyDown(KEY_DOWN)) {
                cameraDistance = Clamp(cameraDistance - cameraZoomSpeed * dt * 50.0f, 50.0f, 600.0f);
            }

            if (IsKeyDown(KEY_X)) {
                cameraAngles.x -= cameraRotationSpeed;
            }
            if (IsKeyDown(KEY_C)) {
                cameraAngles.x += cameraRotationSpeed;
            }
        } else if (cameraMode == FPS) {
            if (!walkMode) {
                UpdateCamera(&g_camera, CAMERA_FIRST_PERSON);
                if (g_camera.position.y < 1.2f) g_camera.position.y = 1.8f;
            }

            Vector3 candidatePos = g_camera.position;

            auto projectOntoSegment = [](const Vector3& a, const Vector3& b, const Vector3& p) {
                Vector3 ab = Vector3Subtract(b, a);
                float abLen2 = Vector3DotProduct(ab, ab);
                if (abLen2 <= 0.0001f) return a;
                float t = Vector3DotProduct(Vector3Subtract(p, a), ab) / abLen2;
                if (t < 0.0f) t = 0.0f;
                if (t > 1.0f) t = 1.0f;
                return Vector3Add(a, Vector3Scale(ab, t));
            };

            float bestDist2 = FLT_MAX;
            Vector3 bestPoint = candidatePos;
            Vector3 bestA = {0,0,0};
            Vector3 bestB = {0,0,0};
            for (const auto& segPtr : network.GetRoadSegments()) {
                RoadSegment* seg = segPtr.get();
                if (!seg || !seg->GetGeometry()) continue;
                auto pts = seg->GetGeometry()->GetPoints();
                for (size_t i = 0; i + 1 < pts.size(); ++i) {
                    Vector3 proj = projectOntoSegment(pts[i], pts[i+1], candidatePos);
                    float dx = proj.x - candidatePos.x;
                    float dz = proj.z - candidatePos.z;
                    float d2 = dx*dx + dz*dz;
                    if (d2 < bestDist2) {
                        bestDist2 = d2;
                        bestPoint = proj;
                        bestA = pts[i];
                        bestB = pts[i+1];
                    }
                }
            }

            const float snapThreshold = 80.0f;
            if (bestDist2 < snapThreshold * snapThreshold) {
                if (!walkMode) {
                    g_camera.position.x = Lerp(g_camera.position.x, bestPoint.x, 0.2f);
                    g_camera.position.z = Lerp(g_camera.position.z, bestPoint.z, 0.2f);
                    g_camera.position.y = bestPoint.y + 1.8f;
                    Vector3 forward = Vector3Normalize(Vector3Subtract(g_camera.target, candidatePos));
                    g_camera.target = Vector3Add(g_camera.position, forward);
                } else {
                    g_camera.position.x = bestPoint.x;
                    g_camera.position.z = bestPoint.z;
                    g_camera.position.y = bestPoint.y + 1.8f;

                    Vector3 roadDir = Vector3Normalize(Vector3Subtract(bestB, bestA));
                    roadDir.y = 0.0f;
                    roadDir = Vector3Normalize(roadDir);

                    Vector2 md = GetMouseDelta();
                    yaw -= md.x * mouseSensitivity;
                    pitch -= md.y * mouseSensitivity;
                    if (pitch > 89.0f) pitch = 89.0f;
                    if (pitch < -89.0f) pitch = -89.0f;

                    Vector3 viewDir = {
                        cosf(pitch * DEG2RAD) * sinf(yaw * DEG2RAD),
                        sinf(pitch * DEG2RAD),
                        cosf(pitch * DEG2RAD) * cosf(yaw * DEG2RAD)
                    };
                    viewDir = Vector3Normalize(viewDir);

                    float speed = IsKeyDown(KEY_LEFT_SHIFT) ? sprintSpeed : moveSpeed;
                    float forwardInput = (IsKeyDown(KEY_W) || IsKeyDown(KEY_Z)) ? 1.0f : (IsKeyDown(KEY_S) ? -1.0f : 0.0f);
                    float strafeInput = (IsKeyDown(KEY_D) ? 1.0f : (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q) ? -1.0f : 0.0f));

                    Vector3 rightOnRoad = Vector3Normalize(Vector3CrossProduct({0,1,0}, roadDir));

                    Vector3 move = Vector3Add(Vector3Scale(roadDir, forwardInput * speed * GetFrameTime()),
                                              Vector3Scale(rightOnRoad, strafeInput * (speed*0.6f) * GetFrameTime()));

                    g_camera.position = Vector3Add(g_camera.position, move);

                    Vector3 projBack = projectOntoSegment(bestA, bestB, g_camera.position);
                    g_camera.position.x = projBack.x;
                    g_camera.position.z = projBack.z;
                    g_camera.position.y = projBack.y + 1.8f;

                    g_camera.target = Vector3Add(g_camera.position, viewDir);
                }
            } else {
                g_camera.position.y = 1.8f;
            }
        } else if (cameraMode == TOPDOWN) {
            float wheel = GetMouseWheelMove();
            topdownMinHeight = 50.0f;
            topdownMaxHeight = 1500.0f;
            topdownHeight = Clamp(topdownHeight - wheel * cameraZoomSpeed * 20.0f, topdownMinHeight, topdownMaxHeight);

            Vector3 panDir = {0, 0, 0};
            float panAmt = topdownPanSpeed * GetFrameTime();
            
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_Z) || IsKeyDown(KEY_UP)) 
                panDir.z -= panAmt;
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) 
                panDir.z += panAmt;
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q) || IsKeyDown(KEY_LEFT)) 
                panDir.x -= panAmt;
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) 
                panDir.x += panAmt;

            g_camera.position.x += panDir.x;
            g_camera.position.z += panDir.z;
            g_camera.target.x += panDir.x;
            g_camera.target.z += panDir.z;

            g_camera.position.y = topdownHeight;
            g_camera.target.y = 0.0f;
            g_camera.up = {0.0f, 0.0f, -1.0f};
            
            EnableCursor();
        } else if (cameraMode == SIMPLE_FPS) {
            UpdateCamera(&g_camera, CAMERA_FIRST_PERSON);
            if (g_camera.position.y < 1.2f) g_camera.position.y = 1.8f;
        } else {
            UpdateCameraFree(&g_camera, &yaw, &pitch, 
                           moveSpeed, sprintSpeed, mouseSensitivity);
        }

        // ==================== INPUT ====================
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        
        if (IsKeyPressed(KEY_C)) {
            cameraMode = (cameraMode == ORBITAL) ? FREE : ORBITAL;
            if (cameraMode == FREE) {
                DisableCursor();
            } else {
                EnableCursor();
            }
        }

        if (IsKeyPressed(KEY_F)) {
            if (cameraMode == FPS) {
                cameraMode = ORBITAL;
                EnableCursor();
                fpsActive = false;
            } else {
                cameraMode = FPS;
                DisableCursor();
                fpsActive = true;
                Vector3 dir = Vector3Subtract(g_camera.target, g_camera.position);
                float len = Vector3Length(dir);
                if (len > 0.001f) {
                    yaw = atan2f(dir.x, dir.z) * RAD2DEG;
                    pitch = asinf(dir.y / len) * RAD2DEG;
                }
            }
        }

        if (IsKeyPressed(KEY_G)) {
            if (cameraMode == SIMPLE_FPS) {
                cameraMode = ORBITAL;
                EnableCursor();
                simpleFpsActive = false;
            } else {
                cameraMode = SIMPLE_FPS;
                DisableCursor();
                simpleFpsActive = true;
                g_camera.position = {networkCenter.x, 1.8f, networkCenter.z};
                g_camera.target = {networkCenter.x, 1.8f, networkCenter.z + 1.0f};
                g_camera.up = {0.0f, 1.0f, 0.0f};
            }
        }

        if (IsKeyPressed(KEY_T)) {
            if (cameraMode == TOPDOWN) {
                cameraMode = ORBITAL;
                g_camera.projection = CAMERA_PERSPECTIVE;
                EnableCursor();
            } else {
                cameraMode = TOPDOWN;
                g_camera.target = networkCenter;
                g_camera.position = {networkCenter.x, topdownHeight, networkCenter.z};
                g_camera.up = {0.0f, 0.0f, -1.0f};
                g_camera.projection = CAMERA_PERSPECTIVE;
                EnableCursor();
            }
        }

        if (IsKeyPressed(KEY_M)) {
            if (cameraMode == FPS) {
                walkMode = !walkMode;
                if (walkMode) DisableCursor(); else EnableCursor();
            }
        }

        if (IsKeyPressed(KEY_R)) {
            cameraFocus = {0.0f, 0.0f, 0.0f};
            cameraDistance = 250.0f;
            cameraAngles = {45.0f * DEG2RAD, 35.0f * DEG2RAD};
            g_camera.position = {200.0f, 150.0f, 200.0f};
            g_camera.target = {0.0f, 0.0f, 0.0f};
        }

        // ==================== ADD VEHICLE ====================
        if (IsKeyPressed(KEY_A)) {
            const auto& nodes = network.GetNodes();
            if (!nodes.empty()) {
                // Sélectionner deux nodes aléatoires
                Node* start = nodes[GetRandomValue(0, (int)nodes.size()-1)].get();
                Node* end = start;
                int tries = 0;
                while (end == start && tries < 10) {
                    end = nodes[GetRandomValue(0, (int)nodes.size()-1)].get();
                    tries++;
                }

                auto nodePath = network.FindPath(start, end);
                if (nodePath.size() >= 2) {
                    // Créer les points du chemin
                    std::vector<Vector3> pathPoints;
                    int sampleCountPerSegment = 12;

                    for (size_t i = 0; i < nodePath.size() - 1; ++i) {
                        Node* a = nodePath[i];
                        Node* b = nodePath[i+1];
                        RoadSegment* connecting = nullptr;
                        for (const auto& segPtr : network.GetRoadSegments()) {
                            RoadSegment* s = segPtr.get();
                            if (!s) continue;
                            if ((s->GetStartNode() == a && s->GetEndNode() == b) ||
                                (s->GetStartNode() == b && s->GetEndNode() == a)) {
                                connecting = s;
                                break;
                            }
                        }
                        if (!connecting) continue;

                        int laneIdx = connecting->GetLanes() > 0 ? connecting->GetLanes()/2 : 0;
                        for (int s = 0; s <= sampleCountPerSegment; ++s) {
                            float t = s / (float)sampleCountPerSegment;
                            pathPoints.push_back(connecting->GetLanePosition(laneIdx, t));
                        }
                    }

                    if (!pathPoints.empty()) {
                        auto car = CarFactory::createCar(
                            StringToCarModel(availableModels[selectedModelIndex]),
                            pathPoints.front(),
                            nextCarId++
                        );

                        // Définir la scale correctement
                        if (pathPoints.size() >= 2) {
                            float dist = Vector3Distance(pathPoints[0], pathPoints[1]);
                            float desiredWidth = fmaxf(1.2f, dist * 0.6f);
                            float scale = desiredWidth / 2.0f;
                            car->setScale(scale);
                        } else {
                            car->setScale(1.0f);  // scale par défaut
                        }

                        // Ajouter l'itinéraire
                        car->setItinerary(pathPoints);
                        trafficMgr.addVehicle(std::move(car));
                    }
                } else {
                    // Fallback : créer un véhicule simple sans itinéraire
                    float angle = GetRandomValue(0, 360) * DEG2RAD;
                    float dist = 40.0f;
                    Vector3 pos = {cosf(angle) * dist, 0.0f, sinf(angle) * dist};
                    
                    auto car = CarFactory::createCar(
                        StringToCarModel(availableModels[selectedModelIndex]),
                        pos,
                        nextCarId++
                    );
                    car->setScale(1.0f);
                    trafficMgr.addVehicle(std::move(car));
                }
            }
        }

        if (!paused)
            trafficMgr.update(dt);

        // ==================== DRAW ====================
        BeginDrawing();
        ClearBackground({25, 25, 35, 255});

        BeginMode3D(g_camera);

        // ==================== ENVIRONNEMENT ====================
        DrawEnvironment(2400.0f);

        // Draw road network
        network.Draw();

        // Draw vehicles
        for (const auto& car : trafficMgr.getVehicles())
            car->draw();

        // Reference axes
        DrawLine3D({0, 0, 0}, {50, 0, 0}, RED);
        DrawLine3D({0, 0, 0}, {0, 50, 0}, GREEN);
        DrawLine3D({0, 0, 0}, {0, 0, 50}, BLUE);

        EndMode3D();

        // ==================== UI ====================
        DrawUIPanel(10, 10, 350, 200, "SMART CITY SIMULATOR");
        DrawText(TextFormat("Vehicles: %d", trafficMgr.getVehicleCount()),
                 20, 50, 18, GREEN);
        DrawText(TextFormat("Time: %.1f s", simTime),
                 20, 75, 16, SKYBLUE);
        DrawText(paused ? "PAUSED" : "RUNNING",
                 20, 100, 16, paused ? RED : GREEN);
        
        const char* camLabel = (cameraMode == ORBITAL) ? "ORBITAL" : (cameraMode == FREE ? "FREE" : (cameraMode == TOPDOWN ? "TOPDOWN" : (cameraMode == SIMPLE_FPS ? "SIMPLE_FPS" : "FPS")));
        DrawText(TextFormat("Camera: %s %s", camLabel, walkMode ? "(WALK)" : ""), 20, 130, 16, YELLOW);

        // ==================== CONTROLS INFO ====================
        DrawUIPanel(10, 220, 350, 420, "CONTROLS");
        DrawText("SPACE   : Pause/Resume", 20, 245, 13, WHITE);
        DrawText("A       : Add Vehicle", 20, 263, 13, WHITE);
        DrawText("C       : Switch Camera Mode", 20, 281, 13, YELLOW);
        DrawText("R       : Reset Camera", 20, 299, 13, WHITE);
        DrawText("ZQSD/WASD   : Pan Camera", 20, 317, 13, GREEN);
        DrawText("E / DOWN    : Zoom Out/In", 20, 335, 13, GREEN);
        DrawText("X / C       : Rotate Camera", 20, 353, 13, GREEN);
        DrawText("SCROLL      : Zoom (Mouse)", 20, 371, 13, WHITE);
        DrawText("RIGHT DRAG  : Rotate (Mouse)", 20, 389, 13, WHITE);
        DrawText("F          : Toggle FPS/Orbit", 20, 407, 13, WHITE);
        DrawText("M          : Toggle Walk-on-Road", 20, 425, 13, WHITE);
        DrawText("T          : Toggle Top-Down", 20, 443, 13, WHITE);
        DrawText("G          : Toggle Simple FPS", 20, 461, 13, WHITE);

        DrawText(TextFormat("FPS: %d | Pos: (%.0f,%.0f,%.0f)", GetFPS(), g_camera.position.x, g_camera.position.y, g_camera.position.z), 850, 870, 12, LIME);

        EndDrawing();
    }

    EnableCursor();
    CloseWindow();
    return 0;
}

// ==================== ORBITAL CAMERA ====================
void UpdateCameraOrbital(Camera3D* camera, float* distance, Vector2* angles,
                         float zoomSpeed, float rotationSpeed, float panSpeed) {

    float wheel = GetMouseWheelMove();
    *distance = Clamp(*distance - wheel * zoomSpeed, 10.0f, 250.0f);

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 d = GetMouseDelta();
        angles->x -= d.x * rotationSpeed;
        angles->y -= d.y * rotationSpeed;
    }

    camera->position = {
        camera->target.x + *distance * sinf(angles->y) * cosf(angles->x),
        camera->target.y + *distance * cosf(angles->y),
        camera->target.z + *distance * sinf(angles->y) * sinf(angles->x)
    };
}

// ==================== FREE CAMERA ====================
void UpdateCameraFree(Camera3D* camera, float* yaw, float* pitch,
                      float moveSpeed, float sprintSpeed, float mouseSensitivity) {
    
    // Mouse look
    Vector2 mouseDelta = GetMouseDelta();
    *yaw -= mouseDelta.x * mouseSensitivity;
    *pitch -= mouseDelta.y * mouseSensitivity;
    
    if (*pitch > 89.0f) *pitch = 89.0f;
    if (*pitch < -89.0f) *pitch = -89.0f;

    // Calculate forward vector
    Vector3 forward = {
        cosf(*pitch * DEG2RAD) * sinf(*yaw * DEG2RAD),
        sinf(*pitch * DEG2RAD),
        cosf(*pitch * DEG2RAD) * cosf(*yaw * DEG2RAD)
    };
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));

    float currentSpeed = IsKeyDown(KEY_LEFT_SHIFT) ? sprintSpeed : moveSpeed;
    float dt = GetFrameTime();

    // Movement
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_Z)) {
        camera->position = Vector3Add(camera->position, Vector3Scale(forward, currentSpeed * dt));
    }
    if (IsKeyDown(KEY_S)) {
        camera->position = Vector3Subtract(camera->position, Vector3Scale(forward, currentSpeed * dt));
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q)) {
        camera->position = Vector3Subtract(camera->position, Vector3Scale(right, currentSpeed * dt));
    }
    if (IsKeyDown(KEY_D)) {
        camera->position = Vector3Add(camera->position, Vector3Scale(right, currentSpeed * dt));
    }

    // Vertical movement
    if (IsKeyDown(KEY_SPACE)) {
        camera->position.y += currentSpeed * dt;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        camera->position.y -= currentSpeed * dt;
        if (camera->position.y < 1.0f) camera->position.y = 1.0f;
    }

    // Update target
    camera->target = Vector3Add(camera->position, forward);
}

// ==================== UI ====================
void DrawUIPanel(int x, int y, int width, int height, const char* title) {
    DrawRectangle(x, y, width, height, Fade(BLACK, 0.7f));
    DrawRectangleLines(x, y, width, height, WHITE);
    DrawText(title, x + 10, y + 5, 16, YELLOW);
}
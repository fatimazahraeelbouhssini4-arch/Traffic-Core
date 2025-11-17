#include "raylib.h"
#include "raymath.h"   // <- nécessaire pour Vector3Normalize, Vector3CrossProduct, etc.
#include <cmath>
#include <vector>

// ---------- Parameters ----------
const int SCREEN_W = 1280;
const int SCREEN_H = 720;

const float ROAD_WIDTH = 12.0f;
const float ROAD_SEGMENT_LENGTH = 10.0f;
const int   ROAD_SEGMENTS = 200;
const float LANE_LINE_WIDTH = 0.4f;
const float LANE_DASH_LENGTH = 4.0f;
const float LANE_DASH_GAP = 6.0f;

const int TREES_PER_SIDE = 40;
const float TREE_SPACING = 12.0f;
const float TREE_OFFSET_FROM_EDGE = 6.0f;
const float TREE_TRUNK_HEIGHT = 2.5f;
const float TREE_TRUNK_WIDTH = 0.6f;
const float TREE_FOLIAGE_RADIUS = 2.0f;
// --------------------------------

struct SimpleTree {
    Vector3 trunkPos;
};

static std::vector<SimpleTree> leftTrees;
static std::vector<SimpleTree> rightTrees;

void DrawSimpleTree(const Vector3 &base, Color trunkColor, Color leafColor) {
    Vector3 trunkPos = { base.x, base.y + TREE_TRUNK_HEIGHT/2.0f, base.z };
    DrawCubeV(trunkPos, { TREE_TRUNK_WIDTH, TREE_TRUNK_HEIGHT, TREE_TRUNK_WIDTH }, trunkColor);
    Vector3 leafPos = { base.x, base.y + TREE_TRUNK_HEIGHT + TREE_FOLIAGE_RADIUS*0.6f, base.z };
    DrawSphere(leafPos, TREE_FOLIAGE_RADIUS, leafColor);
}

void DrawGuardrailAlong(float sideX, float zStart, float zEnd, float spacing) {
    float z = zStart;
    float boxW = 0.2f, boxH = 0.6f, boxL = 0.4f;
    while (z <= zEnd) {
        Vector3 pos = { sideX, boxH/2.0f, z };
        DrawCubeV(pos, { boxW, boxH, boxL }, GRAY);
        z += spacing;
    }
}

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "3D Route - Raylib (fixed)");
    SetTargetFPS(60);

    Camera3D camera;
    camera.position = { 0.0f, 6.0f, 12.0f };
    camera.target   = { 0.0f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Build trees positions
    leftTrees.reserve(TREES_PER_SIDE);
    rightTrees.reserve(TREES_PER_SIDE);
    float startZ = -ROAD_SEGMENT_LENGTH * 5.0f;
    for (int i = 0; i < TREES_PER_SIDE; ++i) {
        float z = startZ + i * TREE_SPACING;
        leftTrees.push_back({ { -ROAD_WIDTH/2.0f - TREE_OFFSET_FROM_EDGE, 0.0f, z } });
        rightTrees.push_back({ {  ROAD_WIDTH/2.0f + TREE_OFFSET_FROM_EDGE, 0.0f, z + 2.0f } });
    }

    Color groundColor = { 100, 160, 80, 255 };
    Color asphaltColor = { 50, 50, 55, 255 };

    float cameraSpeed = 10.0f;
    float yaw = 0.0f;

    while (!WindowShouldClose()) {
        // --- Controls
        Vector3 camDir = Vector3Subtract(camera.target, camera.position);
        camDir = Vector3Normalize(camDir);

        if (IsKeyDown(KEY_W)) {
            Vector3 delta = Vector3Scale(camDir, cameraSpeed * GetFrameTime());
            camera.position = Vector3Add(camera.position, delta);
            camera.target   = Vector3Add(camera.target, delta);
        }
        if (IsKeyDown(KEY_S)) {
            Vector3 delta = Vector3Scale(camDir, cameraSpeed * GetFrameTime());
            camera.position = Vector3Subtract(camera.position, delta);
            camera.target   = Vector3Subtract(camera.target, delta);
        }

        Vector3 right = Vector3Normalize(Vector3CrossProduct(camDir, camera.up));
        if (IsKeyDown(KEY_D)) {
            Vector3 delta = Vector3Scale(right, cameraSpeed * GetFrameTime());
            camera.position = Vector3Add(camera.position, delta);
            camera.target   = Vector3Add(camera.target, delta);
        }
        if (IsKeyDown(KEY_A)) {
            Vector3 delta = Vector3Scale(right, cameraSpeed * GetFrameTime());
            camera.position = Vector3Subtract(camera.position, delta);
            camera.target   = Vector3Subtract(camera.target, delta);
        }

        float rotSpeed = 60.0f * DEG2RAD * GetFrameTime();
        if (IsKeyDown(KEY_LEFT))  yaw -= rotSpeed;
        if (IsKeyDown(KEY_RIGHT)) yaw += rotSpeed;
        if (fabsf(yaw) > 1e-6f) {
            Vector3 dir = Vector3Subtract(camera.target, camera.position);
            float cosA = cosf(yaw), sinA = sinf(yaw);
            float nx = dir.x * cosA - dir.z * sinA;
            float nz = dir.x * sinA + dir.z * cosA;
            camera.target.x = camera.position.x + nx;
            camera.target.z = camera.position.z + nz;
            yaw = 0.0f;
        }
        if (IsKeyDown(KEY_UP)) {
            Vector3 delta = { 0.0f, cameraSpeed * GetFrameTime(), 0.0f };
            camera.position = Vector3Add(camera.position, delta);
            camera.target   = Vector3Add(camera.target, delta);
        }
        if (IsKeyDown(KEY_DOWN)) {
            Vector3 delta = { 0.0f, cameraSpeed * GetFrameTime(), 0.0f };
            camera.position = Vector3Subtract(camera.position, delta);
            camera.target   = Vector3Subtract(camera.target, delta);
        }

        // --- Drawing
        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(camera);

                DrawPlane({0.0f, -0.01f, ROAD_SEGMENT_LENGTH * ROAD_SEGMENTS / 2.0f},
                          {ROAD_SEGMENT_LENGTH * ROAD_SEGMENTS * 0.9f, ROAD_SEGMENT_LENGTH * ROAD_SEGMENTS * 0.9f},
                          groundColor);

                for (int i = -20; i < ROAD_SEGMENTS; ++i) {
                    float z = i * ROAD_SEGMENT_LENGTH;
                    Vector3 planeCenter = { 0.0f, 0.0f, z + ROAD_SEGMENT_LENGTH/2.0f };
                    DrawPlane(planeCenter, { ROAD_WIDTH, ROAD_SEGMENT_LENGTH }, asphaltColor);
                }

                float dashZ = -ROAD_SEGMENT_LENGTH * 20.0f;
                float dashEnd = ROAD_SEGMENT_LENGTH * ROAD_SEGMENTS;
                while (dashZ < dashEnd) {
                    Vector3 dashSize = { LANE_LINE_WIDTH, 0.02f, LANE_DASH_LENGTH };
                    Vector3 dashPos = { 0.0f, 0.01f, dashZ + LANE_DASH_LENGTH/2.0f };
                    DrawCubeV(dashPos, dashSize, WHITE);
                    dashZ += LANE_DASH_LENGTH + LANE_DASH_GAP;
                }

                Vector3 leftLineSize = { 0.05f, 0.02f, ROAD_SEGMENT_LENGTH };
                Vector3 rightLineSize = leftLineSize;
                for (int i = -20; i < ROAD_SEGMENTS; ++i) {
                    float z = i * ROAD_SEGMENT_LENGTH;
                    Vector3 leftLinePos  = { -ROAD_WIDTH/2.0f + 0.5f, 0.01f, z + ROAD_SEGMENT_LENGTH/2.0f };
                    Vector3 rightLinePos = {  ROAD_WIDTH/2.0f - 0.5f, 0.01f, z + ROAD_SEGMENT_LENGTH/2.0f };
                    DrawCubeV(leftLinePos, leftLineSize, WHITE);
                    DrawCubeV(rightLinePos, rightLineSize, WHITE);
                }

                DrawGuardrailAlong(-ROAD_WIDTH/2.0f - 1.8f, -ROAD_SEGMENT_LENGTH*20.0f, ROAD_SEGMENT_LENGTH*(ROAD_SEGMENTS-1), 1.2f);
                DrawGuardrailAlong( ROAD_WIDTH/2.0f + 1.8f, -ROAD_SEGMENT_LENGTH*20.0f, ROAD_SEGMENT_LENGTH*(ROAD_SEGMENTS-1), 1.2f);

                for (const auto &t : leftTrees) DrawSimpleTree(t.trunkPos, {110,70,40,255}, {25,120,30,255});
                for (const auto &t : rightTrees) DrawSimpleTree(t.trunkPos, {110,70,40,255}, {40,150,40,255});

                for (int i = 0; i < 30; ++i) {
                    Vector3 pos = { -60.0f + i * 4.0f, -0.5f, ROAD_SEGMENT_LENGTH * (ROAD_SEGMENTS/2.0f) - i * 3.5f };
                    DrawCubeV(pos, {3.5f, 2.5f, 3.5f}, {40,80,40,255});
                }

                Vector3 carPos = { 0.0f, 0.25f, camera.position.z + 6.0f };
                DrawCubeV(carPos, {1.4f, 0.5f, 2.5f}, RED);

            EndMode3D();

            DrawText("W/S: forward/back  A/D: strafe  Arrows: look/up/down", 10, 10, 20, BLACK);
            DrawText("Move camera to explore the road", 10, 34, 14, DARKGRAY);
            DrawFPS(SCREEN_W - 90, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
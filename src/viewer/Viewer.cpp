#include "Viewer.hpp"
#include "raylib.h"
#include <cmath>

void launchViewer(const vector<AABB>& voxels) {
    InitWindow(1280, 720, "3D Viewer");
    SetTargetFPS(60);

    // camera orbit state
    float angleH = 0.75f;       // horizontal angle (radians)
    float angleV = 0.5f;        // vertical angle
    float dist = 4.0f;          // distance from target
    float orbitSpeed = 0.003f;  // sensitivity mouse drag
    float zoomSpeed = 0.3f;

    Vector3 target = {0.0f, 0.0f, 0.0f};

    // calc center mesh for cam target
    if (!voxels.empty()) {
        float cx=0, cy=0, cz=0;
        for (const AABB& box : voxels) {
            Vec c = centerBox(box);
            cx += c.x; cy += c.y; cz += c.z;
        }
        float n = (float)voxels.size();
        target = {cx/n, cy/n, cz/n};
    }

    Camera3D cam;
    cam.target = target;
    cam.up = {0.0f, 1.0f, 0.0f};
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        // orbit: left mouse drag
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            angleH -= delta.x * orbitSpeed;
            angleV += delta.y * orbitSpeed;
            if (angleV > 1.5f) angleV = 1.5f;
            if (angleV < -1.5f) angleV = -1.5f;
        } else {
            //auto orbit
            angleH += 0.002f;
        }

        // zoom: scroll
        float wheel = GetMouseWheelMove();
        dist -= wheel * zoomSpeed;
        if (dist < 0.5f) dist = 0.5f;
        if (dist > 50.0f) dist = 50.0f;

        // calc camera position from orbit angles
        cam.position = {
            target.x + dist * cosf(angleV) * sinf(angleH),
            target.y + dist * sinf(angleV),
            target.z + dist * cosf(angleV) * cosf(angleH)
        };

        BeginDrawing();
        ClearBackground({30,30,30,255});
        BeginMode3D(cam);

        DrawGrid(20, 0.5f);

        for (const AABB& box : voxels) {
            Vec c = centerBox(box);
            float sx = box.max.x-box.min.x;
            float sy = box.max.y-box.min.y;
            float sz = box.max.z-box.min.z;

            Vector3 pos = {c.x, c.y, c.z};
            Vector3 size = {sx, sy, sz};

            unsigned char r = (unsigned char)(128 + 127*(c.x - target.x));
            unsigned char g = (unsigned char)(128 + 127*(c.y - target.y));
            unsigned char b = (unsigned char)(128 + 127*(c.z - target.z));

            DrawCubeV(pos, size, {r, g, b, 200});
            DrawCubeWiresV(pos, size, {255, 255, 255, 40});
        }

        EndMode3D();
        DrawText("Drag: orbit | Scroll: zoom | ESC: quit", 10, 10, 16, LIGHTGRAY);
        DrawFPS(10, 700);
        EndDrawing();
    }
    CloseWindow();
}

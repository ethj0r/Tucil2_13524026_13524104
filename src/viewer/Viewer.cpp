#include <cmath>
#include "Viewer.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

//utils
static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

//palette
static const int PAL_COUNT = 3;
static const float PAL[PAL_COUNT][3] = {
    { 22/255.0f,  22/255.0f,  22/255.0f},  // #161616 dark
    {255/255.0f, 255/255.0f, 255/255.0f},  // #FFFFFF white
    {  3/255.0f, 103/255.0f, 252/255.0f},  // #0367FC blue
};

static const char* RENDER_NAMES[] = {"Solid+Wire", "Solid", "Wireframe"};

static Color paletteLerp(float t) {
    t = clampf(t, 0.0f, 1.0f);
    float seg = t * (PAL_COUNT - 1);
    int idx = (int)seg;
    if (idx >= PAL_COUNT - 1) idx = PAL_COUNT - 2;
    float frac = seg - (float)idx;

    float r = PAL[idx][0]+(PAL[idx+1][0]-PAL[idx][0])*frac;
    float g = PAL[idx][1]+(PAL[idx+1][1]-PAL[idx][1])*frac;
    float b = PAL[idx][2]+(PAL[idx+1][2]-PAL[idx][2])*frac;

    return {
        (unsigned char)(clampf(r, 0.0f, 1.0f) * 255.0f),
        (unsigned char)(clampf(g, 0.0f, 1.0f) * 255.0f),
        (unsigned char)(clampf(b, 0.0f, 1.0f) * 255.0f),
        230
    };
}

//slider widget
static bool drawSlider(Font font, const char* label, float x, float y,
                       float w, float* value, float lo, float hi,
                       bool* dragging) {
    const float trackH = 4.0f;
    const float knobR  = 7.0f;
    float trackY = y + knobR;
    float norm   = clampf((*value-lo)/(hi-lo), 0.0f, 1.0f);
    float knobX  = x+norm*w;
    float knobY  = trackY+trackH*0.5f;

    Vector2 mouse = GetMousePosition();
    bool overArea = (mouse.x >= x-knobR && mouse.x <= x+w+knobR &&
                     mouse.y >= trackY-knobR*2 && mouse.y <= trackY+trackH+knobR*2);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && overArea) *dragging = true;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *dragging = false;
    if (*dragging) {
        float t = clampf((mouse.x-x)/w, 0.0f, 1.0f);
        *value = lo+t*(hi-lo);
        norm   = t;
        knobX  = x+norm*w;
    }

    DrawRectangleRounded({x, trackY, w, trackH}, 0.5f, 4, {60, 60, 60, 255});
    DrawRectangleRounded({x, trackY, norm * w + 1.0f, trackH}, 0.5f, 4, {3, 103, 252, 255});
    DrawCircleV({knobX, knobY}, knobR, *dragging ? Color{210, 248, 1, 255} : WHITE);
    DrawTextEx(font, label, {x + w + 12, trackY - 6}, 14.0f, 1.0f, LIGHTGRAY);

    return *dragging;
}

//MAIN
void launchViewer(const vector<AABB>& voxels) {
    if (voxels.empty()) return;

    InitWindow(1280, 720, "3D Viewer");
    SetTargetFPS(120);

    Font font = LoadFontEx("../src/viewer/fonts/SF-Pro-Display-Regular.otf", 20, nullptr, 0);
    if (font.texture.id == 0) font = GetFontDefault();

    float bminX = voxels[0].min.x, bmaxX = voxels[0].max.x;
    float bminY = voxels[0].min.y, bmaxY = voxels[0].max.y;
    float bminZ = voxels[0].min.z, bmaxZ = voxels[0].max.z;
    for (const auto& b : voxels) {
        if (b.min.x<bminX) bminX = b.min.x;
        if (b.max.x>bmaxX) bmaxX = b.max.x;
        if (b.min.y<bminY) bminY = b.min.y;
        if (b.max.y>bmaxY) bmaxY = b.max.y;
        if (b.min.z<bminZ) bminZ = b.min.z;
        if (b.max.z>bmaxZ) bmaxZ = b.max.z;
    }
    float cx = (bminX+bmaxX) * 0.5f;
    float cy = (bminY+bmaxY) * 0.5f;
    float cz = (bminZ+bmaxZ) * 0.5f;
    float extX = bmaxX-bminX, extY = bmaxY-bminY, extZ = bmaxZ-bminZ;
    float extent = extX>extY ? (extX>extZ ? extX : extZ) : (extY>extZ ? extY : extZ);
    if (extent<0.001f) extent = 1.0f;
    float halfExtent = extent * 0.5f;

    //initial camera values (for reset)
    const float initAngleH = 0.75f, initAngleV = 0.4f;
    const float initDist = extent * 1.8f;
    const Vector3 initTarget = {0.0f, 0.0f, 0.0f};

    //camera orbit state
    Vector3 target = initTarget;
    float angleH = initAngleH, angleV = initAngleV;
    float dist = initDist;
    float minDist = extent * 0.02f;
    float maxDist = extent * 20.0f;
    float zoomSpeed = extent * 0.05f;

    //orbit speed slider
    float orbitSpeed = 0.002f;
    bool draggingOrbit = false;

    float clipY = 1.0f;
    bool draggingClip = false;

    //explode view
    float explodeTarget = 0.0f;
    float explodeCur = 0.0f;

    //render mode
    RenderMode renderMode = MODE_SOLID_WIRE;

    //ortho toggle
    bool ortho = false;

    Camera3D cam;
    cam.target = target;
    cam.up = {0.0f, 1.0f, 0.0f};
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;
    cam.position = {
        dist * cosf(angleV) * sinf(angleH),
        dist * sinf(angleV),
        dist * cosf(angleV) * cosf(angleH)
    };

    rlSetClipPlanes(extent * 0.0005, extent * 100.0);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool anySliderDrag = draggingOrbit || draggingClip;

        // key toggles
        if (IsKeyPressed(KEY_E))
            explodeTarget = (explodeTarget<0.01f) ? 1.0f : 0.0f;
        explodeCur = lerpf(explodeCur, explodeTarget, clampf(dt * 6.0f, 0.0f, 1.0f));

        if (IsKeyPressed(KEY_W))
            renderMode = (RenderMode)((renderMode + 1) % MODE_COUNT);

        if (IsKeyPressed(KEY_R)) {
            angleH = initAngleH;
            angleV = initAngleV;
            dist = initDist;
            target = initTarget;
            clipY = 1.0f;
            explodeTarget = 0.0f;
        }

        if (IsKeyPressed(KEY_O)) {
            ortho = !ortho;
            cam.projection = ortho ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE;
            cam.fovy = ortho ? extent * 1.5f : 45.0f;
        }

        // mouse input
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !anySliderDrag) {
            Vector2 delta = GetMouseDelta();
            angleH -= delta.x*0.005f;
            angleV += delta.y*0.005f;
            if (angleV>1.5f) angleV = 1.5f;
            if (angleV<-1.5f) angleV = -1.5f;
        } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            angleH += orbitSpeed;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, cam.up));
            Vector3 up = Vector3CrossProduct(right, forward);
            float panSpeed = dist * 0.002f;
            target.x += (-delta.x*right.x+delta.y*up.x)*panSpeed;
            target.y += (-delta.x*right.y+delta.y*up.y)*panSpeed;
            target.z += (-delta.x*right.z+delta.y*up.z)*panSpeed;
        }

        float wheel = GetMouseWheelMove();
        dist -= wheel*zoomSpeed;
        if (dist<minDist) dist = minDist;
        if (dist>maxDist) dist = maxDist;

        //update camera
        cam.target = target;
        cam.position = {
            target.x + dist*cosf(angleV) * sinf(angleH),
            target.y + dist*sinf(angleV),
            target.z + dist*cosf(angleV) * cosf(angleH)
        };

        BeginDrawing();
        ClearBackground({30, 30, 30, 255});
        BeginMode3D(cam);

        rlPushMatrix();
        rlTranslatef(0.0f, -halfExtent, 0.0f);
        DrawGrid(20, extent * 0.1f);
        rlPopMatrix();

        float clipWorldY = -halfExtent + (clipY+1.0f) * 0.5f * extent;

        int visibleCount = 0;
        for (const AABB& box : voxels) {
            Vec c = centerBox(box);
            float sx = box.max.x-box.min.x;
            float sy = box.max.y-box.min.y;
            float sz = box.max.z-box.min.z;

            Vector3 pos = {c.x-cx, c.y-cy, c.z-cz};

            if (pos.y>clipWorldY) continue;
            visibleCount++;

            if (explodeCur>0.001f) {
                float pushDist = explodeCur * extent * 0.5f;
                float len = sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
                if (len > 0.0001f) {
                    pos.x += (pos.x/len) * pushDist;
                    pos.y += (pos.y/len) * pushDist;
                    pos.z += (pos.z/len) * pushDist;
                }
            }

            Vector3 size = {sx, sy, sz};

            float t = clampf((pos.y+halfExtent)/extent, 0.0f, 1.0f);
            float xz = clampf((pos.x+pos.z+extent) / (2.0f*extent), 0.0f, 1.0f);
            t = clampf(t * 0.8f + xz * 0.2f, 0.0f, 1.0f);
            Color col = paletteLerp(t);

            if (renderMode==MODE_SOLID || renderMode==MODE_SOLID_WIRE)
                DrawCubeV(pos, size, col);
            if (renderMode==MODE_WIRE || renderMode==MODE_SOLID_WIRE)
                DrawCubeWiresV(pos, size,
                    renderMode == MODE_WIRE ? col : Color{255, 255, 255, 25});
        }

        EndMode3D();

        //HUD
        float fs = 16.0f;
        float sp = 1.0f;
        DrawTextEx(font, TextFormat("Voxels: %d / %d", visibleCount, (int)voxels.size()),
                   {10, 10}, fs, sp, LIGHTGRAY);

        DrawTextEx(font, "LMB: orbit | RMB: pan | Scroll: zoom",
                   {10, 30}, fs, sp, LIGHTGRAY);

        drawSlider(font, TextFormat("Orbit %.1fx", orbitSpeed / 0.002f),
                   10, 58, 120, &orbitSpeed, 0.0f, 0.01f, &draggingOrbit);

        drawSlider(font, "Cross-section Y",
                   10, 88, 120, &clipY, -1.0f, 1.0f, &draggingClip);

        float badgeY = 118;
        DrawTextEx(font, TextFormat("[W] %s", RENDER_NAMES[renderMode]),
                   {10, badgeY}, 14.0f, sp,
                   renderMode != MODE_SOLID_WIRE ? Color{210, 248, 1, 255} : LIGHTGRAY);
        DrawTextEx(font, TextFormat("[E] Explode %s", explodeTarget > 0.5f ? "ON" : "OFF"),
                   {160, badgeY}, 14.0f, sp,
                   explodeTarget > 0.5f ? Color{210, 248, 1, 255} : LIGHTGRAY);
        DrawTextEx(font, TextFormat("[O] %s", ortho ? "Ortho" : "Persp"),
                   {310, badgeY}, 14.0f, sp,
                   ortho ? Color{210, 248, 1, 255} : LIGHTGRAY);
        DrawTextEx(font, "[R] Reset",
                   {410, badgeY}, 14.0f, sp, LIGHTGRAY);

        DrawFPS(10, 700);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
}
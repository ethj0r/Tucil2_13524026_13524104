#include <cmath>
#include <vector>
#include <algorithm>
#include "Viewer.hpp"
#include "raylib.h"

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

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

//matrix math
Mat4 m4Identity() {
    Mat4 r = {};
    r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = 1.0f;
    return r;
}

Mat4 m4Mul(const Mat4& a, const Mat4& b) {
    Mat4 r = {};
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            for (int k=0; k<4; k++)
                r.m[i][j] += a.m[i][k]*b.m[k][j];
    return r;
}

void m4TransformPoint(const Mat4& mat, float x, float y, float z, float& ox, float& oy, float& oz, float& ow) {
    ox = mat.m[0][0]*x + mat.m[0][1]*y + mat.m[0][2]*z + mat.m[0][3];
    oy = mat.m[1][0]*x + mat.m[1][1]*y + mat.m[1][2]*z + mat.m[1][3];
    oz = mat.m[2][0]*x + mat.m[2][1]*y + mat.m[2][2]*z + mat.m[2][3];
    ow = mat.m[3][0]*x + mat.m[3][1]*y + mat.m[3][2]*z + mat.m[3][3];
}

Mat4 m4LookAt(Vec eye, Vec center, Vec up) {
    Vec f = normalize(subtract(center, eye));
    Vec r = normalize(cross(f, up));
    Vec u = cross(r, f);

    Mat4 m = {};
    m.m[0][0] =  r.x;  m.m[0][1] =  r.y;  m.m[0][2] =  r.z;  m.m[0][3] = -dot(r, eye);
    m.m[1][0] =  u.x;  m.m[1][1] =  u.y;  m.m[1][2] =  u.z;  m.m[1][3] = -dot(u, eye);
    m.m[2][0] = -f.x;  m.m[2][1] = -f.y;  m.m[2][2] = -f.z;  m.m[2][3] =  dot(f, eye);
    m.m[3][3] = 1.0f;
    return m;
}

Mat4 m4Perspective(float fovy, float aspect, float zn, float zf) {
    float t = tanf(fovy*0.5f);
    Mat4 m = {};
    m.m[0][0] = 1.0f/(aspect*t);
    m.m[1][1] = 1.0f/t;
    m.m[2][2] = -(zf+zn)/(zf-zn);
    m.m[2][3] = -(2.0f*zf*zn)/(zf-zn);
    m.m[3][2] = -1.0f;
    return m;
}

Mat4 m4Ortho(float left, float right, float bottom, float top, float zn, float zf) {
    Mat4 m = {};
    m.m[0][0] =  2.0f/(right-left);
    m.m[0][3] = -(right+left)/(right-left);
    m.m[1][1] =  2.0f/(top-bottom);
    m.m[1][3] = -(top+bottom)/(top-bottom);
    m.m[2][2] = -2.0f/(zf-zn);
    m.m[2][3] = -(zf+zn)/(zf-zn);
    m.m[3][3] = 1.0f;
    return m;
}


static const float CUBE_V[8][3] = {
    {0,0,0}, {1,0,0}, {0,0,1}, {1,0,1},
    {0,1,0}, {1,1,0}, {0,1,1}, {1,1,1}
};

static const int CUBE_F[6][4] = {
    {0, 2, 6, 4},
    {1, 5, 7, 3},
    {0, 1, 3, 2},
    {4, 6, 7, 5},
    {0, 4, 5, 1},
    {2, 3, 7, 6},
};

static const float FACE_N[6][3] = {
    {-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}, {0,0,-1}, {0,0,1}
};

struct FaceQuad {
    float sx[4], sy[4]; 
    float depth;
    Color col;
};

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
        norm = t;
        knobX = x+norm*w;
    }

    DrawRectangleRounded({x, trackY, w, trackH}, 0.5f, 4, {60,60,60,255});
    DrawRectangleRounded({x, trackY, norm*w+1.0f, trackH}, 0.5f, 4, {3,103,252,255});
    DrawCircleV({knobX, knobY}, knobR, *dragging ? Color{210,248,1,255} : WHITE);
    DrawTextEx(font, label, {x+w+12, trackY-6}, 14.0f, 1.0f, LIGHTGRAY);
    return *dragging;
}

/**
 * @brief project 3D line ke screen, gambar as 2D line
 */
static void drawLine3D(const Mat4& mvp, float sw, float sh, float x0, float y0, float z0, float x1, float y1, float z1, Color col) {
    float ax, ay, az, aw, bx, by, bz, bw;
    m4TransformPoint(mvp, x0, y0, z0, ax, ay, az, aw);
    m4TransformPoint(mvp, x1, y1, z1, bx, by, bz, bw);
    if (aw<=0.01f || bw<=0.01f) return;

    float sx0 = (ax/aw+1.0f)*0.5f*sw;
    float sy0 = (1.0f-ay/aw)*0.5f*sh;
    float sx1 = (bx/bw+1.0f)*0.5f*sw;
    float sy1 = (1.0f-by/bw)*0.5f*sh;

    DrawLineV({sx0, sy0}, {sx1, sy1}, col);
}

void launchViewer(const vector<AABB>& voxels) {
    if (voxels.empty()) return;

    const float SW=1280, SH=720;
    InitWindow((int)SW, (int)SH, "3D Viewer");
    SetTargetFPS(120);

    Font font = LoadFontEx("../src/viewer/fonts/SF-Pro-Display-Regular.otf", 20, nullptr, 0);
    if (font.texture.id == 0) font = GetFontDefault();

    float bminX = voxels[0].min.x, bmaxX = voxels[0].max.x;
    float bminY = voxels[0].min.y, bmaxY = voxels[0].max.y;
    float bminZ = voxels[0].min.z, bmaxZ = voxels[0].max.z;
    for (const auto& b : voxels) {
        if (b.min.x < bminX) bminX = b.min.x;
        if (b.max.x > bmaxX) bmaxX = b.max.x;
        if (b.min.y < bminY) bminY = b.min.y;
        if (b.max.y > bmaxY) bmaxY = b.max.y;
        if (b.min.z < bminZ) bminZ = b.min.z;
        if (b.max.z > bmaxZ) bmaxZ = b.max.z;
    }
    float sceneCx = (bminX+bmaxX) * 0.5f;
    float sceneCy = (bminY+bmaxY) * 0.5f;
    float sceneCz = (bminZ+bmaxZ) * 0.5f;
    float extX = bmaxX-bminX, extY = bmaxY-bminY, extZ = bmaxZ-bminZ;
    float extent = extX>extY ? (extX>extZ ? extX : extZ) : (extY>extZ ? extY : extZ);
    if (extent<0.001f) extent = 1.0f;
    float halfExt = extent*0.5f;

    //initial camera values (for reset)
    const float initAngleH = 0.75f, initAngleV = 0.4f;
    const float initDist = extent * 1.8f;
    const Vec initTarget = {0, 0, 0};

    //camera orbit state
    Vec camTarget = initTarget;
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
    float orthoSize = extent * 1.5f;

    // projection params
    float fovy = 45.0f*3.14159265f/180.0f;
    float aspect = SW/SH;
    float nearZ = extent*0.001f;
    float farZ  = extent*50.0f;

    //face buffer
    vector<FaceQuad> faces;
    faces.reserve(voxels.size()*3);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool anySliderDrag = draggingOrbit || draggingClip;

        if (IsKeyPressed(KEY_E))
            explodeTarget = (explodeTarget<0.01f) ? 1.0f : 0.0f;
        explodeCur = lerpf(explodeCur, explodeTarget, clampf(dt * 6.0f, 0.0f, 1.0f));

        if (IsKeyPressed(KEY_W))
            renderMode = (RenderMode)((renderMode + 1) % MODE_COUNT);

        if (IsKeyPressed(KEY_R)) {
            angleH = initAngleH; angleV = initAngleV;
            dist = initDist; camTarget = initTarget;
            clipY = 1.0f; explodeTarget = 0.0f;
        }

        if (IsKeyPressed(KEY_O))
            ortho = !ortho;

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
            Vec camPos = {
                camTarget.x + dist*cosf(angleV)*sinf(angleH),
                camTarget.y + dist*sinf(angleV),
                camTarget.z + dist*cosf(angleV)*cosf(angleH)
            };
            Vec fwd = normalize(subtract(camTarget, camPos));
            Vec right = normalize(cross(fwd, {0, 1, 0}));
            Vec up = cross(right, fwd);
            float panSpeed = dist*0.002f;
            camTarget = add(camTarget, scalarMul(right, -delta.x*panSpeed));
            camTarget = add(camTarget, scalarMul(up, delta.y*panSpeed));
        }

        float wheel = GetMouseWheelMove();
        dist -= wheel*zoomSpeed;
        if (dist<minDist) dist = minDist;
        if (dist>maxDist) dist = maxDist;

        Vec camPos = {
            camTarget.x + dist * cosf(angleV) * sinf(angleH),
            camTarget.y + dist * sinf(angleV),
            camTarget.z + dist * cosf(angleV) * cosf(angleH)
        };

        Mat4 view = m4LookAt(camPos, camTarget, {0, 1, 0});
        Mat4 proj;
        if (ortho) {
            float hw = orthoSize*aspect*dist/initDist;
            float hh = orthoSize*dist/initDist;
            proj = m4Ortho(-hw, hw, -hh, hh, nearZ, farZ);
        } else {
            proj = m4Perspective(fovy, aspect, nearZ, farZ);
        }
        Mat4 mvp = m4Mul(proj, view);

        faces.clear();
        float clipWorldY = -halfExt+(clipY+1.0f)*0.5f*extent;
        int visibleCount = 0;
        for (const AABB& box : voxels) {
            Vec c = centerBox(box);
            float bsx = box.max.x-box.min.x;
            float bsy = box.max.y-box.min.y;
            float bsz = box.max.z-box.min.z;

            Vec pos = {c.x-sceneCx, c.y-sceneCy, c.z-sceneCz};

            if (pos.y>clipWorldY) continue;
            visibleCount++;

            //explode
            if (explodeCur>0.001f) {
                float len = length(pos);
                if (len>0.0001f) {
                    float push = explodeCur * extent * 0.5f;
                    pos = add(pos, scalarMul(normalize(pos), push));
                }
            }

            float wx[8], wy[8], wz[8];
            for (int v=0; v<8; v++) {
                wx[v] = pos.x+(CUBE_V[v][0]-0.5f)*bsx;
                wy[v] = pos.y+(CUBE_V[v][1]-0.5f)*bsy;
                wz[v] = pos.z+(CUBE_V[v][2]-0.5f)*bsz;
            }

            float t = clampf((pos.y+halfExt)/extent, 0.0f, 1.0f);
            float xz = clampf((pos.x+pos.z+extent) / (2.0f*extent), 0.0f, 1.0f);
            t = clampf(t * 0.8f + xz * 0.2f, 0.0f, 1.0f);
            Color col = paletteLerp(t);

            Vec toCamera = subtract(camPos, pos);

            for (int fi=0; fi<6; fi++) {
                float ndot = FACE_N[fi][0]*toCamera.x + FACE_N[fi][1]*toCamera.y + FACE_N[fi][2]*toCamera.z;
                if (ndot<=0.0f) continue;

                FaceQuad fq;
                fq.col = col;
                bool clipped = false;
                float depthSum = 0.0f;

                for (int vi=0; vi<4; vi++) {
                    int idx = CUBE_F[fi][vi];
                    float cpx, cpy, cpz, cpw;
                    m4TransformPoint(mvp, wx[idx], wy[idx], wz[idx], cpx, cpy, cpz, cpw);

                    if (cpw<=0.001f) { clipped = true; break; }

                    float invW = 1.0f/cpw;
                    fq.sx[vi] = (cpx*invW+1.0f)*0.5f*SW;
                    fq.sy[vi] = (1.0f-cpy*invW)*0.5f*SH;
                    depthSum += cpz*invW;
                }

                if (clipped) continue;
                fq.depth = depthSum*0.25f;
                faces.push_back(fq);
            }
        }

        sort(faces.begin(), faces.end(), [](const FaceQuad& a, const FaceQuad& b) {
            return a.depth > b.depth;
        });

        BeginDrawing();
        ClearBackground({30, 30, 30, 255});

        {
            Color gridCol = {60, 60, 60, 255};
            int gridN = 20;
            float step = extent*0.1f;
            float gridHalf = gridN*0.5f*step;
            for (int i=-gridN/2; i<=gridN/2; i++) {
                float p = i*step;
                drawLine3D(mvp, SW, SH, p, -halfExt, -gridHalf, p, -halfExt,  gridHalf, gridCol);
                drawLine3D(mvp, SW, SH, -gridHalf, -halfExt, p, gridHalf, -halfExt, p, gridCol);
            }
        }

        for (const FaceQuad& fq : faces) {
            if (renderMode==MODE_SOLID || renderMode==MODE_SOLID_WIRE) {
                Vector2 a = {fq.sx[0], fq.sy[0]};
                Vector2 b = {fq.sx[1], fq.sy[1]};
                Vector2 c2 = {fq.sx[2], fq.sy[2]};
                Vector2 d = {fq.sx[3], fq.sy[3]};
                DrawTriangle(a, b, c2, fq.col);
                DrawTriangle(a, c2, d, fq.col);
            }
            if (renderMode==MODE_WIRE || renderMode==MODE_SOLID_WIRE) {
                Color wc = (renderMode==MODE_WIRE) ? fq.col : Color{255,255,255,40};
                for (int i=0; i<4; i++) {
                    int j = (i+1)%4;
                    DrawLineV({fq.sx[i], fq.sy[i]}, {fq.sx[j], fq.sy[j]}, wc);
                }
            }
        }

        float fs = 16.0f, sp = 1.0f;
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
                   renderMode != MODE_SOLID_WIRE ? Color{210,248,1,255} : LIGHTGRAY);
        DrawTextEx(font, TextFormat("[E] Explode %s", explodeTarget > 0.5f ? "ON" : "OFF"),
                   {160, badgeY}, 14.0f, sp,
                   explodeTarget > 0.5f ? Color{210,248,1,255} : LIGHTGRAY);
        DrawTextEx(font, TextFormat("[O] %s", ortho ? "Ortho" : "Persp"),
                   {310, badgeY}, 14.0f, sp,
                   ortho ? Color{210,248,1,255} : LIGHTGRAY);
        DrawTextEx(font, "[R] Reset",
                   {410, badgeY}, 14.0f, sp, LIGHTGRAY);

        DrawFPS(10, 700);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
}
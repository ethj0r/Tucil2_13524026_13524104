#pragma once
#include <cmath>

// Vector 3D
struct Vec {
    float x, y, z;
};

// segitiga (3 titik) untuk mesh
struct Triangle {
    Vec v1, v2, v3;
};

// axis-aligned bounding box untuk node octree
struct AABB {
    Vec min, max;
};

// ------------- Vector -------------

// operator plus
Vec plus(Vec a, Vec b);

// operator minus
Vec minus(Vec a, Vec b);

// operator scalar multiply
Vec scalarMul(Vec a, float b);

// panjang vektor
float length(Vec a);

// normalisasi vektor
Vec normalize(Vec a);

// Dot product
float dot(Vec a, Vec b);

// Cross product
Vec cross(Vec a, Vec b);

// ------------- Triangle -------------

// center segitiga
Vec center(const Triangle& a);

// normal segitiga
Vec normal(const Triangle& a);

// bounding box segitiga
AABB triangleAABB(const Triangle& tri);

// ------------- AABB -------------

// titik tengah box
Vec center(const AABB& box);

// cek apakah titik berada di dalam box
bool containsPoint(const AABB& box, const Vec& point);

// cek overlap dua box
bool overlaps(const AABB& a, const AABB& b);

// cek intersection triangle dengan AABB
bool intersectsTriangleAABB(const Triangle& tri, const AABB& box);




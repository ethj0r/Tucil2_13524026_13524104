#pragma once
#include "Triangle.hpp"

struct BoundingBox {
    Vec min, max;
};

// buat bounding box yang contain segitiga
BoundingBox triangleBox(const Triangle& tri);

// titik tengah box
Vec center(const BoundingBox& box);

// cek apakah titik berada di dalam box
bool containsPoint(const BoundingBox& box, const Vec& point);

// cek overlap 2 box
bool overlaps(const BoundingBox& a, const BoundingBox& b);

// cek intersection triangle dengan Bounding Box
bool intersectsTriangleBox(const Triangle& tri, const BoundingBox& box);
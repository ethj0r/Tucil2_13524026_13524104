#pragma once
#include <vector>
#include "Triangle.hpp"

struct AABB {
    Vec min, max;
};

float min3(float a, float b, float c);
float max3(float a, float b, float c);

/**
 * @brief test overlap proyeksi segitiga dan box pada satu sumbu
 * @param axis Sumbu proyeksi yang diuji pada pemisahan SAT (Separating Axis Theorem)
 * @param v1, v2, v3 titik 1 2 3 segitiga relatif terhadap pusat box
 * @param halfSize 1/2 ukuran box
 * @return true kl proyeksi segitiga dan box overlap pada sumbu tersebut
 */
bool axisTest(const Vec& axis, const Vec& v1, const Vec& v2, const Vec& v3, const Vec& halfSize);

/**
 * @brief make bounding box terkecil yang mengelilingi satu segitiga
 * @param tri
 * @return AABB yang contain segitiga
 */
AABB triangleBox(const Triangle& tri);

/**
 * @brief calc titik tengah bounding box
 * @param box Bounding box.
 * @return vec titik tengah = (min + max) / 2.
 */
Vec centerBox(const AABB& box);

/**
 * @brief ngebagi bounding box jadi 8 sub-box (octant)
 * @param box bounding box yg akan dibagi
 * @return std::vector<AABB> isinya 8 sub-box
 */
std::vector<AABB> splitBox(const AABB& box);

/**
 * @brief Cek apakah titik berada di dalam bounding box (inclusive).
 * @param box Bounding box.
 * @param point Titik yang dicek.
 * @return true jika titik di dalam box.
 */
bool containsPoint(const AABB& box, const Vec& point);

/**
 * @brief cek dua bounding box saling overlap
 * @return true jika overlap.
 */
bool overlaps(const AABB& a, const AABB& b);

/**
 * @brief cek intersection segitiga dengan bounding box pake SAT
 * @return true jika segitiga intersect/menyentuh box.
 */
bool intersectsTriangleBox(const Triangle& tri, const AABB& box);

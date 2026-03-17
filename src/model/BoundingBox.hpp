#pragma once
#include <vector>
#include "Triangle.hpp"

struct BoundingBox {
    Vec min, max;
};

/**
 * @brief make bounding box terkecil yang mengelilingi satu segitiga
 * @param tri
 * @return BoundingBox yang contain segitiga
 */
BoundingBox triangleBox(const Triangle& tri);

/**
 * @brief calc bounding box dari seluruh daftar vertex
 * @param vertices
 * @return BoundingBox contain semua vertex
 */
BoundingBox meshBox(const std::vector<Vec>& vertices);

/**
 * @brief norm bounding box jadi kubus
 * @param box
 * @return BoundingBox kubus
 */
BoundingBox normalizeBox(const BoundingBox& box);

/**
 * @brief ngebagi bounding box jadi 8 sub-box
 * @param box bounding box yg akan dibagi
 * @return std::vector<BoundingBox> isinya 8 sub-box
 */
std::vector<BoundingBox> splitBox(const BoundingBox& box);

/**
 * @brief calc titik tengah bounding box
 * @param box Bounding box.
 * @return vec titik tengah = (min + max) / 2.
 */
Vec center(const BoundingBox& box);

/**
 * @brief Cek apakah titik berada di dalam bounding box (inclusive).
 * @param box Bounding box.
 * @param point Titik yang dicek.
 * @return true jika titik di dalam box.
 */
bool containsPoint(const BoundingBox& box, const Vec& point);

/**
 * @brief cek dua bounding box saling overlap
 * @return true jika overlap.
 */
bool overlaps(const BoundingBox& a, const BoundingBox& b);

/**
 * @brief cek intersection segitiga dengan bounding box pake SAT
 * @return true jika segitiga intersect/menyentuh box.
 */
bool intersectsTriangleBox(const Triangle& tri, const BoundingBox& box);
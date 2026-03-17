#pragma once
#include <vector>
#include "Triangle.hpp"

struct BoundingBox {
    Vec min, max;
};

/**
 * @brief Mengembalikan nilai terkecil dari 3 bilangan.
 * @param a Bilangan 1.
 * @param b Bilangan 2.
 * @param c Bilangan 3.
 * @return Nilai minimum dari a, b, dan c.
 */
float min3(float a, float b, float c);

/**
 * @brief Mengembalikan nilai terbesar dari 3 bilangan.
 * @param a Bilangan 1.
 * @param b Bilangan 2.
 * @param c Bilangan 3.
 * @return Nilai maksimum dari a, b, dan c.
 */
float max3(float a, float b, float c);

/**
 * @brief Menguji overlap proyeksi segitiga dan box pada satu sumbu.
 * @param axis Sumbu proyeksi yang diuji pada pemisahan SAT (Separating Axis Theorem).
 * @param v1 Titik 1 segitiga relatif terhadap pusat box.
 * @param v2 Titik 2 segitiga relatif terhadap pusat box.
 * @param v3 Titik 3 segitiga relatif terhadap pusat box.
 * @param halfSize Setengah ukuran box.
 * @return true jika proyeksi segitiga dan box overlap pada sumbu tersebut.
 */
bool axisTest(const Vec& axis, const Vec& v1, const Vec& v2, const Vec& v3, const Vec& halfSize);

/**
 * @brief make bounding box terkecil yang mengelilingi satu segitiga
 * @param tri
 * @return BoundingBox yang contain segitiga
 */
BoundingBox triangleBox(const Triangle& tri);

/**
 * @brief calc titik tengah bounding box
 * @param box Bounding box.
 * @return vec titik tengah = (min + max) / 2.
 */
Vec centerBox(const BoundingBox& box);

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

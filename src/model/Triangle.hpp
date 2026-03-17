#pragma once
#include "Vector.hpp"

struct Triangle {
    Vec v1, v2, v3;
};

/**
 * @brief calc titik tengah (centroid) segitiga
 * @return Vec titik tengah = (v1 + v2 + v3) / 3.
 */
Vec centerTri(const Triangle& a);

/**
 * @brief calc normal segitiga (unit vector tegak lurus permukaan)
 * @return vec unit normal
 */
Vec normalTri(const Triangle& a);

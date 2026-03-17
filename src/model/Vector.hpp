#pragma once
#include <cmath>
using namespace std;

struct Vec {
    float x, y, z;
};

Vec add(Vec a, Vec b);
Vec subtract(Vec a, Vec b);

/**
 * @brief perkalian vektor dgn skalar
 * @param a vektor
 * @param b skalar
 * @return vec hasil a * b.
 */
Vec scalarMul(Vec a, float b);

/**
 * @brief calc magnitude vektor
 * @param a vektor
 * @return float panjang vektor
 */
float length(Vec a);

/**
 * @brief norm vektor jadi unit vector
 * @return vec unit vector, atau {0,0,0} kl panjang ~0
 */
Vec normalize(Vec a);

/**
 * @brief dot product dua vektor
 * @return float hasil dot product.
 */
float dot(Vec a, Vec b);

/**
 * @brief cross product dua vektor
 * @return Vec hasil cross product axb
 */
Vec cross(Vec a, Vec b);
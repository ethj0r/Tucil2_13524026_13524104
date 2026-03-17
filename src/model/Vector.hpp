#pragma once
#include <cmath>
using namespace std;

// Vector 3D
struct Vec {
    float x, y, z;
};

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

// dot product
float dot(Vec a, Vec b);

// cross product
Vec cross(Vec a, Vec b);
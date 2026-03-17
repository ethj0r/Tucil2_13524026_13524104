#include "Triangle.hpp"

Vec centerTri(const Triangle& a) {
    return scalarMul(add(add(a.v1, a.v2), a.v3), 1.0f / 3.0f);
}

Vec normalTri(const Triangle& a) {
    Vec edge1 = subtract(a.v2, a.v1);
    Vec edge2 = subtract(a.v3, a.v1);
    return normalize(cross(edge1, edge2));
}
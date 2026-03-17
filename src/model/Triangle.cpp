#include "Triangle.hpp"

Vec center(const Triangle& a) {
    return scalarMul(plus(plus(a.v1, a.v2), a.v3), 1.0f / 3.0f);
}

Vec normal(const Triangle& a) {
    Vec edge1 = minus(a.v2, a.v1);
    Vec edge2 = minus(a.v3, a.v1);
    return normalize(cross(edge1, edge2));
}
#pragma once
#include "Vector.hpp"

// segitiga (3 titik) untuk mesh
struct Triangle {
    Vec v1, v2, v3;
};

// center segitiga
Vec centerTri(const Triangle& a);

// normal segitiga
Vec normalTri(const Triangle& a);
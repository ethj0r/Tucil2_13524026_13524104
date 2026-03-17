#pragma once
#include "Vector.hpp"

// segitiga (3 titik) untuk mesh
struct Triangle {
    Vec v1, v2, v3;
};

// center segitiga
Vec center(const Triangle& a);

// normal segitiga
Vec normal(const Triangle& a);
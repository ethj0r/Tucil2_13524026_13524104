#include "Vector.hpp"

Vec plus(Vec a, Vec b){
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec minus(Vec a, Vec b){
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec scalarMul(Vec a, float b){
    return {a.x * b, a.y * b, a.z * b};
}

float length(Vec a){
    return sqrt(dot(a, a));
}

Vec normalize(Vec a){
    float len = length(a);
    if(len < 1e-6f){
        return {0.0f, 0.0f, 0.0f};
    }

    return scalarMul(a, 1.0f / len);
}

float dot(Vec a, Vec b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vec cross(Vec a, Vec b){
    return {(a.y * b.z) - (b.y * a.z), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)};
}
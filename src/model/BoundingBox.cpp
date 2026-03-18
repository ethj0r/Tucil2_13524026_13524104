#include "BoundingBox.hpp"

// ----------- Helpers -----------

// min dari 3 bilangan 
float min3(float a, float b, float c) {
    if (a<=b && a<=c) return a;
    if (b<=c) return b;
    return c;
}

// max dari 3 bilangan
float max3(float a, float b, float c) {
    if (a>=b && a>=c) return a;
    if (b>=c) return b;
    return c;
}

// Separating axis theorem
bool axisTest(const Vec& axis, const Vec& v1, const Vec& v2, const Vec& v3, const Vec& halfSize) {
    if (fabs(axis.x) < 1e-6f && fabs(axis.y) < 1e-6f && fabs(axis.z) < 1e-6f){ // toleransi float
        return true;
    }

    // Proyeksi titik segitiga
    float p1 = dot(v1, axis);
    float p2 = dot(v2, axis);
    float p3 = dot(v3, axis);

    float minProj = min3(p1, p2, p3);
    float maxProj = max3(p1, p2, p3);

    // Hitung radius box (batas max kotak)
    float radius = halfSize.x * fabs(axis.x) + halfSize.y * fabs(axis.y) + halfSize.z * fabs(axis.z);

    // apakah ada celah antara segitiga & kotak
    return !(minProj > radius || maxProj < -radius);
}

BoundingBox triangleBox(const Triangle& tri) {
    BoundingBox box;
    box.min.x = min3(tri.v1.x, tri.v2.x, tri.v3.x);
    box.min.y = min3(tri.v1.y, tri.v2.y, tri.v3.y);
    box.min.z = min3(tri.v1.z, tri.v2.z, tri.v3.z);
    box.max.x = max3(tri.v1.x, tri.v2.x, tri.v3.x);
    box.max.y = max3(tri.v1.y, tri.v2.y, tri.v3.y);
    box.max.z = max3(tri.v1.z, tri.v2.z, tri.v3.z);
    return box;
}

Vec centerBox(const BoundingBox& box) {
    return scalarMul(add(box.min, box.max), 0.5f);
}

std::vector<BoundingBox> splitBox(const BoundingBox& box) {
    Vec mid = centerBox(box);
    std::vector<BoundingBox> children(8);

    for (int i=0; i<8; i++) {
        children[i].min.x = (i & 1) ? mid.x : box.min.x;
        children[i].min.y = (i & 2) ? mid.y : box.min.y;
        children[i].min.z = (i & 4) ? mid.z : box.min.z;
        children[i].max.x = (i & 1) ? box.max.x : mid.x;
        children[i].max.y = (i & 2) ? box.max.y : mid.y;
        children[i].max.z = (i & 4) ? box.max.z : mid.z;
    }

    return children;
}

bool containsPoint(const BoundingBox& box, const Vec& point) {
    return point.x >= box.min.x && point.x <= box.max.x && point.y >= box.min.y && point.y <= box.max.y && 
           point.z >= box.min.z && point.z <= box.max.z;
}

bool overlaps(const BoundingBox& a, const BoundingBox& b) {
    return a.min.x <= b.max.x && a.max.x >= b.min.x && a.min.y <= b.max.y && a.max.y >= b.min.y &&
           a.min.z <= b.max.z && a.max.z >= b.min.z;
}

bool intersectsTriangleBox(const Triangle& tri, const BoundingBox& box) {
    if (containsPoint(box, tri.v1) || containsPoint(box, tri.v2) || containsPoint(box, tri.v3)){
        return true;
    }

    BoundingBox triBox = triangleBox(tri);
    if (!overlaps(triBox, box)) {
        return false;
    }

    Vec boxCenter = centerBox(box);
    Vec halfSize = scalarMul(subtract(box.max, box.min), 0.5f);

    // Normalisasi koordinat
    Vec v1 = subtract(tri.v1, boxCenter);
    Vec v2 = subtract(tri.v2, boxCenter);
    Vec v3 = subtract(tri.v3, boxCenter);

    // vektor tepi segitiga
    Vec edges[3] = {subtract(v2, v1), subtract(v3, v2), subtract(v1, v3)};

    // vektor satuan
    Vec boxAxes[3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; 

    // sumbu cross product tepi segitiga dengan sumbu kotak (satuan)
    for (const Vec& edge : edges) {
        for (const Vec& boxAxis : boxAxes) {
            if (!axisTest(cross(edge, boxAxis), v1, v2, v3, halfSize)) {
                return false;
            }
        }
    }

    // sumbu utama
    if (max3(v1.x, v2.x, v3.x) < -halfSize.x || min3(v1.x, v2.x, v3.x) > halfSize.x) {
        return false;
    }
    if (max3(v1.y, v2.y, v3.y) < -halfSize.y || min3(v1.y, v2.y, v3.y) > halfSize.y) {
        return false;
    }
    if (max3(v1.z, v2.z, v3.z) < -halfSize.z || min3(v1.z, v2.z, v3.z) > halfSize.z) {
        return false;
    }

    // sumbu normal segitiga 
    Vec triangleNormal = cross(edges[0], edges[1]);
    if (!axisTest(triangleNormal, v1, v2, v3, halfSize)) {
        return false;
    }

    return true;
}
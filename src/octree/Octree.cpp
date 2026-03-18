#include "Octree.hpp"

void buildOctree(const AABB& box, const vector<Triangle>& triangles,
    int depth, int maxDepth, OctreeResult& result) {

    if ((int)result.nodesPerDepth.size()<=maxDepth) {
        result.nodesPerDepth.resize(maxDepth+1, 0);
        result.prunedPerDepth.resize(maxDepth+1, 0);
    }
    result.nodesPerDepth[depth]++;

    vector<Triangle> intersecting;
    for (const Triangle& tri : triangles) {
        if (intersectsTriangleBox(tri, box)) intersecting.push_back(tri);
    }
    if (intersecting.empty()) {
        result.prunedPerDepth[depth]++;
        return;
    }

    if (depth==maxDepth) {
        result.voxels.push_back(box);
        return;
    }

    vector<AABB> children = splitBox(box);
    for (const AABB& child : children) {
        buildOctree(child, intersecting, depth+1, maxDepth, result);
    }

}
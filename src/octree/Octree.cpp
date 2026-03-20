#include "Octree.hpp"

namespace {
    const int THREAD_LIMIT = 1;  // Limit thread set to 1
}

void buildOctree(const BoundingBox& box, const vector<Triangle>& triangles,
    int depth, int maxDepth, OctreeResult& result) {

    // resize di root saja (tidak perlu lock karena masih 1 thread)
    if(depth == 0){
        result.nodesPerDepth.resize(maxDepth+1, 0);
        result.prunedPerDepth.resize(maxDepth+1, 0);
    }

    // Lock semua critical section
    {
        lock_guard<mutex> lock(result.mtx);
        result.nodesPerDepth[depth]++;
    }

    vector<Triangle> intersecting;
    for (const Triangle& tri : triangles) {
        if (intersectsTriangleBox(tri, box)) intersecting.push_back(tri);
    }

    if (intersecting.empty()) {
        lock_guard<mutex> lock(result.mtx);
        result.prunedPerDepth[depth]++;
        return;
    }

    if (depth==maxDepth) {
        lock_guard<mutex> lock(result.mtx);
        result.voxels.push_back(box);
        return;
    }

    vector<BoundingBox> children = splitBox(box);

    if(depth < THREAD_LIMIT){
        vector<thread> threads;
        for(const BoundingBox& child : children){
            threads.emplace_back([child, &intersecting, depth, maxDepth, &result]() {
                buildOctree(child, intersecting, depth + 1, maxDepth, result);
            });
        }

        for (thread& t : threads){ // gabung semua thread
            t.join();
        }

    } else {
        for (const BoundingBox& child : children) {
            buildOctree(child, intersecting, depth+1, maxDepth, result);
        }
    }

}
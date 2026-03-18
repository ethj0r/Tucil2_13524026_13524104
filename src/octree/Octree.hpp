#pragma once
#include <vector>

#include "../model/BoundingBox.hpp"
#include "../model/Triangle.hpp"

/**
 * @brief res build octree
 * save list voxel (leaf bounding box)
 * save stats node/pruned per depth
 */
struct OctreeResult {
    vector<BoundingBox> voxels;
    vector<int> nodesPerDepth;
    vector<int> prunedPerDepth;
};
 
/**
 * @brief build octree rekursif pake divide and conquer
 * @param box current bouding box
 * @param triangles list triangle yg mungkin intersect
 * @param depth current depth (start dari 0)
 * @param maxDepth max depth dari octree
 * @param result output voxels+stats
 */
void buildOctree(const BoundingBox& box, const vector<Triangle>& triangles,
    int depth, int maxDepth, OctreeResult& result);

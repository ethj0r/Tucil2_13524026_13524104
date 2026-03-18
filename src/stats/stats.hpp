#pragma once
#include <string>
#include <vector>

#include "../model/Mesh.hpp"
#include "../octree/Octree.hpp"

struct Stats {
    int maxDepth = 0;
    double execTime = 0.0;

    int voxelCount = 0;
    int vertexCount = 0;
    int faceCount = 0;

    vector<int> nodesPerDepth;
    vector<int> prunedPerDepth;
};

/**
 * @brief Hitung statistik dari mesh, voxel, dan octree.
 * @param mesh input mesh.
 * @param voxels list voxel leaf dari octree.
 * @param octree Hasil octree.
 * @param maxDepth Depth maksimal octree.
 * @param execTime Waktu eksekusi (ms).
 * @return Stats dengan data mesh, voxel, dan octree.
 */
Stats buildStats(const vector<BoundingBox>& voxels, const OctreeResult& octree, int maxDepth, double execTime);

/**
 * @brief Print stats voxelization.
 * @param stats Stats yang telah memiliki info.
 * @param outputPath Path output file.
 * @return void
 */
void printStats(const Stats& stats, const string& outputPath);

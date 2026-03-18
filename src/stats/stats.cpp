#include "stats.hpp"
#include <iostream>

Stats buildStats(const vector<BoundingBox>& voxels, const OctreeResult& octree, int maxDepth, double execTime) {
    Stats stats;
    stats.maxDepth = maxDepth;
    stats.execTime = execTime;

    stats.voxelCount = (int)voxels.size();
    stats.vertexCount = stats.voxelCount * 8;
    stats.faceCount = stats.voxelCount * 12;

    stats.nodesPerDepth = octree.nodesPerDepth;
    stats.prunedPerDepth = octree.prunedPerDepth;

    return stats;
}

void printStats(const Stats& stats, const string& outputPath) {
    cout << "\nVOXELIZATION REPORT\n";
    cout << "Voxel count    : " << stats.voxelCount << "\n";
    cout << "Vertex count   : " << stats.vertexCount << "\n";
    cout << "Face count     : " << stats.faceCount << "\n";

    cout << "\nOctree nodes per depth:\n";
    for (int i = 0; i <= stats.maxDepth; i++) {
        cout << "  " << i << " : " << stats.nodesPerDepth[i] << "\n";
    }

    cout << "\nPruned nodes per depth:\n";
    for (int i = 0; i <= stats.maxDepth; i++) {
        cout << "  " << i << " : " << stats.prunedPerDepth[i] << "\n";
    }

    cout << "\nOctree depth   : " << stats.maxDepth << "\n";
    cout << "Execution time : " << stats.execTime << " ms\n";
    cout << "Output saved   : " << outputPath << "\n";
}

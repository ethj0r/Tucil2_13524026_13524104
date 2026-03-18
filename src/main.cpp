#include <iostream>
#include <chrono>
#include "model/Mesh.hpp"
#include "exporter/ObjExporter.hpp"
#include "octree/Octree.hpp"
#include "stats/stats.hpp"

using namespace std;

int main() {
    string filepath;
    cout << "Path ke .obj file: ";
    getline(cin, filepath);

    string depthStr;
    cout << "Max depth octree: ";
    getline(cin, depthStr);
    int maxDepth = stoi(depthStr);

    if (maxDepth<1) {
        cerr << "Error: depth harus >= 1" << endl;
        return 1;
    }

    // load mesh
    Mesh mesh;
    if (!loadObj(filepath, mesh)) {
        cerr << "Error: gagal load " << filepath << endl;
        return 1;
    }

    cout << "\nMesh loaded: " << mesh.vertices.size() << " vertices, " << mesh.triangles.size() << " triangles\n";

    auto start = chrono::high_resolution_clock::now();

    OctreeResult octree;
    buildOctree(mesh.bounds, mesh.triangles, 0, maxDepth, octree);

    auto end = chrono::high_resolution_clock::now();
    double execTime = chrono::duration<double, milli>(end - start).count();

    // export
    string outputPath = "../test/output.obj";
    exportObj(octree.voxels, outputPath);

    // build & print stats
    Stats stats = buildStats(octree.voxels, octree, maxDepth, execTime);
    printStats(stats, outputPath);

    return 0;
}

#include <iostream>
#include <chrono>
#include "model/Mesh.hpp"
#include "exporter/ObjExporter.hpp"
#include "viewer/Viewer.hpp"
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

    //export
    string basename = filepath;
    size_t lastSlash = basename.find_last_of("/\\");
    if (lastSlash!=string::npos) basename = basename.substr(lastSlash+1);
    size_t dotPos = basename.rfind('.');
    if (dotPos!=string::npos) basename = basename.substr(0, dotPos);
    string outputPath = "../test/"+basename+"_output.obj";
    exportObj(octree.voxels, outputPath);

    // build & print stats
    Stats stats = buildStats(octree.voxels, octree, maxDepth, execTime);
    printStats(stats, outputPath);

    launchViewer(octree.voxels);

    return 0;
}

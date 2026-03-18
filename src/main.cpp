#include <iostream>
#include <chrono>
#include "model/Mesh.hpp"
#include "octree/Octree.hpp"
#include "parser/ObjParser.hpp"
#include "exporter/ObjExporter.hpp"

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

    // build octree
    auto start = chrono::high_resolution_clock::now();

    OctreeResult octree;
    buildOctree(mesh.bounds, mesh.triangles, 0, maxDepth, octree);

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end-start).count();

    // export
    string outputPath = "../test/output.obj";
    int totalVertex = exportObj(octree.voxels, outputPath);
    int totalFaces = (int)octree.voxels.size()*12;

    // report
    cout << "\nVOXELIZATION REPORT\n";
    cout << "Voxel count    : " << octree.voxels.size() << "\n";
    cout << "Vertex count   : " << totalVertex << "\n";
    cout << "Face count     : " << totalFaces << "\n";

    cout << "\nOctree nodes per depth:\n";
    for (int i=0; i<=maxDepth; i++) {
        cout << "  " << i << " : " << octree.nodesPerDepth[i] << "\n";
    }

    cout << "\nPruned nodes per depth:\n";
    for (int i=0; i<=maxDepth; i++) {
        cout << "  " << i << " : " << octree.prunedPerDepth[i] << "\n";
    }

    cout << "\nOctree depth   : " << maxDepth << "\n";
    cout << "Execution time : " << ms << " ms\n";
    cout << "Output saved   : " << outputPath << "\n";

    return 0;
}

#include "ObjExporter.hpp"

int exportObj(const vector<BoundingBox>& voxels, const string filepath){
    ofstream file(filepath);
    if (!file.is_open()) throw runtime_error("Cannot open output file: "+filepath);
    
    file << "# Voxelized OBJ - " << voxels.size() << " voxels\n";
    int vertexCnt=0;

    for (const BoundingBox& box : voxels){
        float x0=box.min.x, y0=box.min.y, z0=box.min.z;
        float x1=box.max.x, y1=box.max.y, z1=box.max.z;

        // 8 vertex per kubus
        file << "v " << x0 << " " << y0 << " " << z0 << "\n";
        file << "v " << x1 << " " << y0 << " " << z0 << "\n";
        file << "v " << x1 << " " << y1 << " " << z0 << "\n";
        file << "v " << x0 << " " << y1 << " " << z0 << "\n";
        file << "v " << x0 << " " << y0 << " " << z1 << "\n";
        file << "v " << x1 << " " << y0 << " " << z1 << "\n";
        file << "v " << x1 << " " << y1 << " " << z1 << "\n";
        file << "v " << x0 << " " << y1 << " " << z1 << "\n";

        int b = vertexCnt+1;

        // 12 triangle (2 per face)
        file << "f " << b   << " " << b+2 << " " << b+1 << "\n";
        file << "f " << b   << " " << b+3 << " " << b+2 << "\n";
        file << "f " << b+4 << " " << b+5 << " " << b+6 << "\n";
        file << "f " << b+4 << " " << b+6 << " " << b+7 << "\n";
        file << "f " << b   << " " << b+1 << " " << b+5 << "\n";
        file << "f " << b   << " " << b+5 << " " << b+4 << "\n";
        file << "f " << b+2 << " " << b+3 << " " << b+7 << "\n";
        file << "f " << b+2 << " " << b+7 << " " << b+6 << "\n";
        file << "f " << b+1 << " " << b+2 << " " << b+6 << "\n";
        file << "f " << b+1 << " " << b+6 << " " << b+5 << "\n";
        file << "f " << b   << " " << b+4 << " " << b+7 << "\n";
        file << "f " << b   << " " << b+7 << " " << b+3 << "\n";

        vertexCnt+=8;
    }
    file.close();
    return vertexCnt;
}
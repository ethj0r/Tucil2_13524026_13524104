#include "ObjParser.hpp"

/**
 * @brief ekstrak vertex index dari satu token face.
 * @param token 1 token dari baris face (e.g., "3/1/2").
 * @param vertexCount jml vertex yg udh parsed (untuk resolve negative index).
 * @return int Index vertex 0-based.
 */
static int parseFaceIndex(const string& token, int vertexCount){
    string indexStr;
    size_t slash = token.find('/');
    if (slash!=string::npos) {
        indexStr = token.substr(0,slash);
    } else {
        indexStr = token;
    }

    int idx = stoi(indexStr);
    if (idx<0) {
        idx = vertexCount+idx; 
    } else {
        idx-=1; 
    }

    return idx;
}

ParseResult parseObj(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filepath);
    }

    ParseResult result;
    string line;

    while (getline(file, line)) {
        if (!line.empty()&&line.back()=='\r') line.pop_back();
        if (line.empty()) continue;

        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix=="v") {
            float x, y, z;
            if (!(iss>>x>>y>>z)) {
                throw runtime_error("Invalid vertex line: " + line);
            }
            result.vertices.push_back({x, y, z});

        } else if (prefix=="f") {
            vector<int> indices;
            string token;
            while (iss >> token) {
                int idx = parseFaceIndex(token, (int)result.vertices.size());
                if (idx<0 || idx>=(int)result.vertices.size()) {
                    throw runtime_error("Face index out of range: " + line);
                }
                indices.push_back(idx);
            }

            if (indices.size()<3) {
                throw runtime_error("Face with less than 3 vertices: " + line);
            }

            for (size_t i=1; i+1<indices.size(); i++) {
                Triangle tri;
                tri.v1 = result.vertices[indices[0]];
                tri.v2 = result.vertices[indices[i]];
                tri.v3 = result.vertices[indices[i+1]];
                result.triangles.push_back(tri);
            }
        }
    }

    if (result.vertices.empty()) {
        throw runtime_error("No vertices found in file: " + filepath);
    }
    if (result.triangles.empty()) {
        throw runtime_error("No faces found in file: " + filepath);
    }

    return result;
}

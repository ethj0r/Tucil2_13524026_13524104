#include <iostream>
#include "parser/ObjParser.hpp"
using namespace std;

int main() {
    string filepath;
    cout << "Path ke .obj file: ";
    getline(cin, filepath);

    try {
        ParseResult result = parseObj(filepath);
        cout << endl;
        cout << "Vertices  : " << result.vertices.size() << endl;
        cout << "Triangles : " << result.triangles.size() << endl;

        cout << "\nVERTICES" << endl;
        for (size_t i = 0; i < result.vertices.size(); i++) {
            Vec& v = result.vertices[i];
            cout << "  v[" << i << "] = (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
        }

        cout << "\nTRIANGLES" << endl;
        for (size_t i=0; i<result.triangles.size(); i++) {
            Triangle& t = result.triangles[i];
            cout << "  t[" << i << "] = ("
                 << t.v1.x << "," << t.v1.y << "," << t.v1.z << ") ("
                 << t.v2.x << "," << t.v2.y << "," << t.v2.z << ") ("
                 << t.v3.x << "," << t.v3.y << "," << t.v3.z << ")" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

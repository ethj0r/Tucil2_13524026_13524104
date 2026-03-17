#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "../model/Vector.hpp"
#include "../model/Triangle.hpp"

using namespace std;

/**
 * @brief res parsing file .obj
 *
 * save list vertex dan triangle yang telah resolved
 * dari index ke koordinat Vec.
 */
struct ParseResult {
    vector<Vec> vertices;
    vector<Triangle> triangles;
};

/**
 * @brief parse file .obj dan ekstrak vertices serta triangles
 *
 * @param filepath path ke file .obj yang akan di-parse.
 * @return ParseResult berisi vertices dan triangles.
 * @throws runtimerror kl file !found, vertex/face ga valid,
 *         idx out of range, or file tidak mengandung vertex/face
 */
ParseResult parseObj(const string& filepath);

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "../model/BoundingBox.hpp"

using namespace std;

/**
 * @brief export list voxl ke file .obj
 * setiap voxel bakal dirender sbg kubus dgn 8 vertex dan 12 triangle (6 faces)
 * 
 * @param voxels list bounding box dari leaf octree
 * @param filepath output path
 * @return total vertex yg ditulis
 */
int exportObj(const vector<AABB>& voxels, const string filepath);
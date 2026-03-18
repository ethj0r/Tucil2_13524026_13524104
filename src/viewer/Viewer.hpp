#pragma once
#include <vector>
#include "../model/BoundingBox.hpp"

/**
 * @brief launch 3D viewer window
 * @param voxels lst boundingbox dari leaf octree
 */
void launchViewer(const vector<AABB>& voxels);
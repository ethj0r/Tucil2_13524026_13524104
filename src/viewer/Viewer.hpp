#pragma once
#include <vector>
#include "../model/BoundingBox.hpp"

enum RenderMode { MODE_SOLID_WIRE, MODE_SOLID, MODE_WIRE, MODE_COUNT };

/**
 * @brief launch 3D viewer window
 * @param voxels list of AABB from octree leaf nodes
 */
void launchViewer(const vector<AABB>& voxels);
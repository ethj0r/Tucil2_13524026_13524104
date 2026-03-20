#pragma once
#include <vector>
#include "../model/BoundingBox.hpp"
using namespace std;

enum RenderMode { MODE_SOLID_WIRE, MODE_SOLID, MODE_WIRE, MODE_COUNT };
struct Mat4 {
    float m[4][4];
};
Mat4 m4Identity();

/**
 * @brief multiply dua matrix 4x4
 * @return Mat4 hasil perkalian
 */
Mat4 m4Mul(const Mat4& a, const Mat4& b);

/**
 * @brief transform titik 3D (w=1) thru matrix 4x4
 * @param mat combined MVP matrix
 * @param x, y, z world-space position
 * @param ox, oy, oz, ow output clip-space coordinates
 */
void m4TransformPoint(const Mat4& mat, float x, float y, float z, float& ox, float& oy, float& oz, float& ow);

/**
 * @brief build view matrix dari posisi cam (manual lookAt)
 * @param eye posisi cam di world space
 * @param center dots yang dilihat cam
 * @param up
 * @return Mat4 view matrix
 */
Mat4 m4LookAt(Vec eye, Vec center, Vec up);

/**
 * @brief build perspective projection matrix
 * @param fovy field of view vertikal (rad))
 * @param aspect ratio width/height
 * @param zn near clipping plane (positif)
 * @param zf far clipping plane (positif)
 * @return Mat4 projection matrix
 */
Mat4 m4Perspective(float fovy, float aspect, float zn, float zf);

/**
 * @brief build orthographic projection matrix
 * @param left, right batas horizontal
 * @param bottom, top batas vertikal
 * @param zn, zf near dan far plane
 * @return Mat4 orthographic matrix
 */
Mat4 m4Ortho(float left, float right, float bottom, float top, float zn, float zf);

/**
 * @brief launch interactive 3D voxel viewer (software renderer)
 * @param voxels leaf nodes dari octree (AABB)
 */
void launchViewer(const vector<AABB>& voxels);
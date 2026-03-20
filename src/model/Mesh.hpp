#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "BoundingBox.hpp"
#include "../parser/ObjParser.hpp"

/**
 * @brief representasi mesh hasil parsing file .obj
 *
 * menyimpan data yang siap dipakai untuk octree
 * daftar vertex, daftar triangle, dan bounding box global.
 */
struct Mesh {
	vector<Vec> vertices;
	vector<Triangle> triangles;
	AABB bounds;
	bool hasBounds = false;
};

/**
 * @brief Cek apakah mesh tidak memiliki triangle atau vertices.
 * @param mesh Mesh yang akan dicek.
 * @return true jika mesh kosong, false jika ada triangle atau vertices.
 */
bool isMeshEmpty(const Mesh& mesh);

/**
 * @brief Parse file OBJ dan isi data mesh.
 * @param filePath Path file OBJ yang akan dibaca.
 * @param outMesh Output mesh hasil parsing.
 * @return true jika parsing berhasil dan minimal ada 1 triangle dan vertice valid, false jika gagal.
 */
bool loadObj(const string& filePath, Mesh& outMesh);

/**
 * @brief Hitung bounding box global dari triangle mesh.
 * @param mesh Mesh yang akan dihitung ulang nilai bounds-nya.
 * @return void
 */
void buildMeshBounds(Mesh& mesh);

/**
 * @brief Membuat bounding box menjadi kubus
 * @param mesh Mesh yang akan dibuat menjadi bentuk kubus
 * @return void
 */
void makeBoundsCubic(Mesh& mesh);

#include "Mesh.hpp"

bool isMeshEmpty(const Mesh& mesh) {
	return mesh.vertices.empty() || mesh.triangles.empty();
}

bool loadObj(const string& filePath, Mesh& outMesh) {

	// Init mesh
	outMesh.vertices.clear();
	outMesh.triangles.clear();
	outMesh.hasBounds = false;

	try {
		// Parse file
		ParseResult parsed = parseObj(filePath);
		outMesh.vertices = move(parsed.vertices);
		outMesh.triangles = move(parsed.triangles);

		buildMeshBounds(outMesh);
		makeBoundsCubic(outMesh);
		return !isMeshEmpty(outMesh);
	} catch (exception& e) {
		cerr << "Caught exception: " << e.what() << endl;
		return false;
	}
}

void buildMeshBounds(Mesh& mesh) {
	mesh.hasBounds = false;

	// Jika mesh kosong, tidak ada bounding box yang bisa dibentuk
	if (isMeshEmpty(mesh)){
		return;
	}

	// Inisialisasi min dan max
	const Triangle& first = mesh.triangles[0];
	mesh.bounds.min = first.v1;
	mesh.bounds.max = first.v1;

	for (const Triangle& tri : mesh.triangles){
		const Vec points[3] = {tri.v1, tri.v2, tri.v3};
		for (const Vec& p : points){

			// Update min dan max per iterasi
			if (p.x < mesh.bounds.min.x){
				mesh.bounds.min.x = p.x;
			}

			if (p.y < mesh.bounds.min.y){
				mesh.bounds.min.y = p.y;
			}

			if (p.z < mesh.bounds.min.z){
				mesh.bounds.min.z = p.z;
			}

			if (p.x > mesh.bounds.max.x){
				mesh.bounds.max.x = p.x;
			}
			if (p.y > mesh.bounds.max.y){
				mesh.bounds.max.y = p.y;
			}
			if (p.z > mesh.bounds.max.z){
				mesh.bounds.max.z = p.z;
			}
		}
	}

	mesh.hasBounds = true;
}

void makeBoundsCubic(Mesh& mesh){
	if(!mesh.hasBounds){
		return;
	}

	// titik tengah box
	Vec center = centerBox(mesh.bounds);

	// panjang box pada masing2 sumbu
	Vec size = subtract(mesh.bounds.max, mesh.bounds.min);

	// sisi kubus diambil dari sumbu terbesar agar seluruh mesh ter-cover
	float cubeSize = max3(size.x, size.y, size.z);
	float halfSize = cubeSize * 0.5f;

	mesh.bounds.min = {center.x - halfSize, center.y - halfSize, center.z - halfSize};
	mesh.bounds.max = {center.x + halfSize, center.y + halfSize, center.z + halfSize};
}
#include "Mesh.hpp"

bool isMeshEmpty(const Mesh& mesh) {
	return mesh.vertices.empty() || mesh.triangles.empty();
}

bool loadObj(const string& filePath, Mesh& outMesh) {

    // init Mesh
	outMesh.vertices.clear();
	outMesh.triangles.clear();
	outMesh.hasBounds = false;

	try {
		ParseResult parsed = parseObj(filePath);
		outMesh.vertices = move(parsed.vertices);
		outMesh.triangles = move(parsed.triangles);
		buildMeshBounds(outMesh);
		return !isMeshEmpty(outMesh);
	} catch (exception& e) {
        cerr << "Caught exception: " << e.what() << endl;
		return false;
	}
}

void buildMeshBounds(Mesh& mesh) {
	mesh.hasBounds = false;

    // mesh kosong, langsung selesai
	if (isMeshEmpty(mesh)){
		return;
	}

	const Triangle& first = mesh.triangles[0];
	mesh.bounds.min = first.v1;
	mesh.bounds.max = first.v1;

	for (const Triangle& tri : mesh.triangles){
		const Vec points[3] = {tri.v1, tri.v2, tri.v3};
		for (const Vec& p : points){

            // mencari min dan max dari sumbu x,y,z
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
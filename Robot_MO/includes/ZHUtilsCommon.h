#pragma once
#include "GLUtils.hpp" // MeshObject  és  Vertex

//
// Geometriát létrehozó függvények 
//

// Kocka 
inline MeshObject<Vertex> createCube()
{
	MeshObject<Vertex> mesh;

	mesh.vertexArray = {
		// Elülső oldala 
		{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bal lent 
		{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Jobb lent 
		{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // Jobb fent 
		{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // Bal fent 

		// Hátulsó oldala 
		{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}, // Bal lent 
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},// Jobb lent 
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}, // Jobb fent 
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  // Bal fent 

		// Bal oldala 
		{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// Bal lent 
		{{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Jobb lent 
		{{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Jobb fent 
		{{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Bal fent 

		// Jobb oldala 
		{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Bal lent 
		{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Jobb lent 
		{{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Jobb fent 
		{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},   // Bal fent 

		// Felső oldala 
		{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Bal lent 
		{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // Jobb lent 
		{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Jobb fent 
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // Bal fent 

		// Alsó oldala 
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},// Bal lent 
		{{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, // Jobb lent 
		{{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},  // Jobb fent 
		{{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // Bal fent 
	};

	mesh.indexArray = {// Elülső oldala 
		0, 1, 2, 2, 3, 0,
		// Hátulsó oldala 
		4, 5, 6, 6, 7, 4,
		// Bal oldala 
		8, 9, 10, 10, 11, 8,
		// Jobb oldala 
		12, 13, 14, 14, 15, 12,
		// Felső oldala 
		16, 17, 18, 18, 19, 16,
		// Alsó oldala 
		20, 21, 22, 22, 23, 20 };

	return mesh;
}

// Quad 
inline MeshObject<Vertex> createQuad()
{
	MeshObject<Vertex> quadMeshCPU;

	quadMeshCPU.vertexArray =
	{
		{ glm::vec3( -0.5, 0 ,  0.5),glm::vec3( 0.0, 1.0, 0.0 ), glm::vec2( 0.0, 0.0 ) },
		{ glm::vec3(  0.5, 0 ,  0.5),glm::vec3( 0.0, 1.0, 0.0 ), glm::vec2( 1.0, 0.0 ) },
		{ glm::vec3( -0.5, 0 , -0.5),glm::vec3( 0.0, 1.0, 0.0 ), glm::vec2( 0.0, 1.0 ) },
		{ glm::vec3(  0.5, 0 , -0.5),glm::vec3( 0.0, 1.0, 0.0 ), glm::vec2( 1.0, 1.0 ) },
		{ glm::vec3( -0.5, 0 ,  0.5),glm::vec3( 0.0,-1.0, 0.0 ), glm::vec2( 0.0, 0.0 ) },
		{ glm::vec3(  0.5, 0 ,  0.5),glm::vec3( 0.0,-1.0, 0.0 ), glm::vec2( 1.0, 0.0 ) },
		{ glm::vec3( -0.5, 0 , -0.5),glm::vec3( 0.0,-1.0, 0.0 ), glm::vec2( 0.0, 1.0 ) },
		{ glm::vec3(  0.5, 0 , -0.5),glm::vec3( 0.0,-1.0, 0.0 ), glm::vec2( 1.0, 1.0 ) }
	};

	quadMeshCPU.indexArray =
	{
		0, 1, 2,
		1, 3, 2,
		6, 7, 5,
		6, 5, 4
	};

	return quadMeshCPU;
}

// Pyramid
MeshObject<Vertex> createPyramid()
{
	MeshObject<Vertex> pyramidMeshCPU;

	static constexpr float SQRT_2 = glm::root_two<float>();

	glm::vec3 normals[5] = {
	  glm::vec3(0,-1,0),
	  glm::normalize(glm::vec3(0,       1,  -SQRT_2)),
	  glm::normalize(glm::vec3(-SQRT_2, 1,        0)),
	  glm::normalize(glm::vec3(0,       1,   SQRT_2)),
	  glm::normalize(glm::vec3(SQRT_2,  1,        0))
	};

	pyramidMeshCPU.vertexArray =
	{
		// Alap 
		{ glm::vec3(-1.0f,    0.0f, -1.0f), normals[0], glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 1.0f,    0.0f, -1.0f), normals[0], glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f,    0.0f,  1.0f), normals[0], glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 1.0f,    0.0f,  1.0f), normals[0], glm::vec2(1.0f, 1.0f) },

		// Csúcs 
		{ glm::vec3(0.0f,  SQRT_2,  0.0f), normals[1], glm::vec2(0.5, 1.0f) },
		{ glm::vec3(0.0f,  SQRT_2,  0.0f), normals[2], glm::vec2(0.5, 1.0f) },
		{ glm::vec3(0.0f,  SQRT_2,  0.0f), normals[3], glm::vec2(0.5, 1.0f) },
		{ glm::vec3(0.0f,  SQRT_2,  0.0f), normals[4], glm::vec2(0.5, 1.0f) },

		// Lapok 
		{ glm::vec3( 1.0f,    0.0f, -1.0f), normals[1], glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f,    0.0f, -1.0f), normals[1], glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(-1.0f,    0.0f, -1.0f), normals[2], glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f,    0.0f,  1.0f), normals[2], glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(-1.0f,    0.0f,  1.0f), normals[3], glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 1.0f,    0.0f,  1.0f), normals[3], glm::vec2(1.0f, 0.0f) },

		{ glm::vec3( 1.0f,    0.0f,  1.0f), normals[4], glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 1.0f,    0.0f, -1.0f), normals[4], glm::vec2(1.0f, 0.0f) }
	};

	pyramidMeshCPU.indexArray =
	{
		0,1,2,
		2,1,3,
		4,8,9,
		5,10,11,
		6,12,13,
		7,14,15
	};

	return pyramidMeshCPU;
}
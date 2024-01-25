#pragma once

#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>

#include "Cube.h"

class Renderer
{
	static constexpr int FACE_COUNT = cubeSolver::Cube::Face::faceCount;

	static constexpr int VTX_PER_FACE = 4;
	static constexpr int CUBE_VTX_COUNT = FACE_COUNT * VTX_PER_FACE;
	static constexpr int TRIANGLES_PER_FACE = 2;
	static constexpr int VTX_PER_TRIANGLE = 3;
	static constexpr int CUBE_INDICE_COUNT = FACE_COUNT * TRIANGLES_PER_FACE * VTX_PER_TRIANGLE;

	ALLEGRO_VERTEX cubeVtx[CUBE_VTX_COUNT];
	int cubeIndices[CUBE_INDICE_COUNT] = {
		 0,  1,  2, // front
		 0,  2,  3,
		 4,  5,  6, // right
		 4,  6,  7,
		 8,  9, 10, // back
		 8, 10, 11,
		12, 13, 14, // left
		12, 14, 15,
		16, 17, 18, // top
		16, 18, 19,
		20, 21, 22, // bottom
		20, 22, 23
	};

public:
	Renderer();
	void drawCube(ALLEGRO_BITMAP* texture, cubeSolver::Cube cube);

private:
	void updateCubeVertices(float x, float y, float z);
	void updateCubeColours(const cubeSolver::Cube::Colours& colours, bool revert);
};

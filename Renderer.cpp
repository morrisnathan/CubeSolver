#include "Renderer.h"
#include "Cube.h"

Renderer::Renderer()
{
	ALLEGRO_COLOR c = al_map_rgb_f(1, 1, 1);

	for (int colour = 0; colour < COLOUR_COUNT; ++colour)
	{
		// Front face
		cubeVtx[colour * CUBE_VTX_COUNT] =      { -1, -1,  1, (float)colour * 250,         0, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 1] =  { -1,  1,  1, (float)colour * 250,       250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 2] =  {  1,  1,  1, (float)colour * 250 + 250, 250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 3] =  {  1, -1,  1, (float)colour * 250 + 250,   0, c };
		// Right face
		cubeVtx[colour * CUBE_VTX_COUNT + 4] =  {  1, -1,  1, (float)colour * 250,         0, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 5] =  {  1,  1,  1, (float)colour * 250,       250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 6] =  {  1,  1, -1, (float)colour * 250 + 250, 250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 7] =  {  1, -1, -1, (float)colour * 250 + 250,   0, c };
		// Back face
		cubeVtx[colour * CUBE_VTX_COUNT + 8] =  {  1, -1, -1, (float)colour * 250,         0, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 9] =  {  1,  1, -1, (float)colour * 250,       250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 10] = { -1,  1, -1, (float)colour * 250 + 250, 250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 11] = { -1, -1, -1, (float)colour * 250 + 250,   0, c };
		// Left face
		cubeVtx[colour * CUBE_VTX_COUNT + 12] = { -1, -1, -1, (float)colour * 250,         0, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 13] = { -1,  1, -1, (float)colour * 250,       250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 14] = { -1,  1,  1, (float)colour * 250 + 250, 250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 15] = { -1, -1,  1, (float)colour * 250 + 250,   0, c };
		// Top face
		cubeVtx[colour * CUBE_VTX_COUNT + 16] = { -1,  1,  1, (float)colour * 250,         0, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 17] = { -1,  1, -1, (float)colour * 250,       250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 18] = {  1,  1, -1, (float)colour * 250 + 250, 250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 19] = {  1,  1,  1, (float)colour * 250 + 250,   0, c };
		// Bottom face
		cubeVtx[colour * CUBE_VTX_COUNT + 20] = { -1, -1, -1, (float)colour * 250,         0, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 21] = { -1, -1,  1, (float)colour * 250,       250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 22] = {  1, -1,  1, (float)colour * 250 + 250, 250, c };
		cubeVtx[colour * CUBE_VTX_COUNT + 23] = {  1, -1, -1, (float)colour * 250 + 250,   0, c };

		for (int j = 0; j < CUBE_VTX_COUNT; ++j)
		{
			cubeVtx[colour * CUBE_VTX_COUNT + j].x = cubeVtx[colour * CUBE_VTX_COUNT + j].x * 0.97;
			cubeVtx[colour * CUBE_VTX_COUNT + j].y = cubeVtx[colour * CUBE_VTX_COUNT + j].y * 0.97;
			cubeVtx[colour * CUBE_VTX_COUNT + j].z = cubeVtx[colour * CUBE_VTX_COUNT + j].z * 0.97;
		}
	}
}

void Renderer::drawCube(ALLEGRO_BITMAP* texture, float x, float y, float z, float theta, cubeSolver::Cube::Colour colours[FACE_COUNT])
{
	ALLEGRO_TRANSFORM t;

	al_identity_transform(&t);
	al_rotate_transform_3d(&t, 0, 1, 0, theta);
	al_rotate_transform_3d(&t, 1, 0, 0, 0.5);
	al_translate_transform_3d(&t, x, y, z);
	al_use_transform(&t);

	int indices[CUBE_INDICE_COUNT];
	for (int i = 0; i < CUBE_INDICE_COUNT; ++i)
	{
		cubeSolver::Cube::Colour colour = colours[i / FACE_COUNT];
		indices[i] = cubeIndices[i] + colour * CUBE_VTX_COUNT;
	}

	for (int i = 0; i < 3; ++i)
	{
		updateCubeVertices(i == 0 ? -2 : 2, 0, 0);
		for (int j = 0; j < 3; ++j)
		{
			updateCubeVertices(0, j == 0 ? -2 : 2, 0);
			for (int k = 0; k < 3; ++k)
			{
				updateCubeVertices(0, 0, k == 0 ? -2 : 2);
				al_draw_indexed_prim(cubeVtx, nullptr, texture, indices, CUBE_INDICE_COUNT, ALLEGRO_PRIM_TRIANGLE_LIST);
			}
			updateCubeVertices(0, 0, -2);
		}
		updateCubeVertices(0, -2, 0);
	}
	updateCubeVertices(-2, 0, 0);
}

void Renderer::updateCubeVertices(float x, float y, float z)
{
	for (int colour = 0; colour < COLOUR_COUNT; ++colour)
	{
		for (int j = 0; j < CUBE_VTX_COUNT; ++j)
		{
			cubeVtx[colour * CUBE_VTX_COUNT + j].x += x;
			cubeVtx[colour * CUBE_VTX_COUNT + j].y += y;
			cubeVtx[colour * CUBE_VTX_COUNT + j].z += z;
		}
	}
}

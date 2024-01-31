#include "Renderer.h"
#include "Cube.h"

Renderer::Renderer()
{
	ALLEGRO_COLOR c = al_map_rgb_f(1, 1, 1);

	// Use the same vertices for each block in the cube with default translation and
	// texture coordinates. The process to draw the cube will then update the coordinates
	// for each block before drawing and then reset them afterwards.

	// Front face
	cubeVtx[0] =  { -1, -1,  1, 0,     0, c };
	cubeVtx[1] =  { -1,  1,  1, 0,   250, c };
	cubeVtx[2] =  {  1,  1,  1, 250, 250, c };
	cubeVtx[3] =  {  1, -1,  1, 250,   0, c };
	// Right face
	cubeVtx[4] =  {  1, -1,  1, 0,     0, c };
	cubeVtx[5] =  {  1,  1,  1, 0,   250, c };
	cubeVtx[6] =  {  1,  1, -1, 250, 250, c };
	cubeVtx[7] =  {  1, -1, -1, 250,   0, c };
	// Back face
	cubeVtx[8] =  {  1, -1, -1, 0,     0, c };
	cubeVtx[9] =  {  1,  1, -1, 0,   250, c };
	cubeVtx[10] = { -1,  1, -1, 250, 250, c };
	cubeVtx[11] = { -1, -1, -1, 250,   0, c };
	// Left face
	cubeVtx[12] = { -1, -1, -1, 0,     0, c };
	cubeVtx[13] = { -1,  1, -1, 0,   250, c };
	cubeVtx[14] = { -1,  1,  1, 250, 250, c };
	cubeVtx[15] = { -1, -1,  1, 250,   0, c };
	// Top face
	cubeVtx[16] = { -1,  1,  1, 0,     0, c };
	cubeVtx[17] = { -1,  1, -1, 0,   250, c };
	cubeVtx[18] = {  1,  1, -1, 250, 250, c };
	cubeVtx[19] = {  1,  1,  1, 250,   0, c };
	// Bottom face
	cubeVtx[20] = { -1, -1, -1, 0,     0, c };
	cubeVtx[21] = { -1, -1,  1, 0,   250, c };
	cubeVtx[22] = {  1, -1,  1, 250, 250, c };
	cubeVtx[23] = {  1, -1, -1, 250,   0, c };

	// Add space between each block
	for (int j = 0; j < CUBE_VTX_COUNT; ++j)
	{
		cubeVtx[j].x *= 0.97;
		cubeVtx[j].y *= 0.97;
		cubeVtx[j].z *= 0.97;
	}
}

void Renderer::drawCube(ALLEGRO_BITMAP* texture, Cube cube)
{
	ALLEGRO_TRANSFORM t; // cube parent transform
	const Transform& ct = cube.getTransform();

	al_identity_transform(&t);
	al_rotate_transform_3d(&t, 0, 1, 0, ct.rotation.y);
	al_rotate_transform_3d(&t, 1, 0, 0, ct.rotation.x);
	al_translate_transform_3d(&t, ct.translation.x, ct.translation.y, ct.translation.z);

	ALLEGRO_TRANSFORM rt; // slice local transform
	const Cube::SliceRotation& sr = cube.getSliceRotation();

	if (cube.isSliceRotating())
	{
		al_identity_transform(&rt);
		if (sr.axis == Cube::SliceRotation::Axis::x)
			al_rotate_transform_3d(&rt, 1, 0, 0, sr.angle);		
		else if (sr.axis == Cube::SliceRotation::Axis::y)
			al_rotate_transform_3d(&rt, 0, 1, 0, sr.angle);
		else if (sr.axis == Cube::SliceRotation::Axis::z)
			al_rotate_transform_3d(&rt, 0, 0, 1, sr.angle);
		al_compose_transform(&rt, &t);
	}

	for (int z = 0; z < 3; ++z)
	{
		// Draw blocks from front slice to back slice
		updateCubeVertices(0, 0, z == 0 ? 2 : -2);

		for (int y = 0; y < 3; ++y)
		{
			// Draw slices from top row to bottom row
			updateCubeVertices(0, y == 0 ? 2 : -2, 0);

			for (int x = 0; x < 3; ++x)
			{
				if (cube.isSliceRotating() &&
					(sr.slice == Cube::SliceRotation::Slice::all ||
						(sr.axis == Cube::SliceRotation::Axis::x && x == sr.slice) ||
						(sr.axis == Cube::SliceRotation::Axis::y && y == sr.slice) ||
						(sr.axis == Cube::SliceRotation::Axis::z && z == sr.slice)))
					al_use_transform(&rt);
				else
					al_use_transform(&t);

				// Draw rows from left block to right block
				updateCubeVertices(x == 0 ? -2 : 2, 0, 0);
				const Cube::Colours& colours = cube.getColours()[z * 9 + y * 3 + x];

				updateCubeColours(colours, false);
				al_draw_indexed_prim(cubeVtx, nullptr, texture, cubeIndices, CUBE_INDICE_COUNT, ALLEGRO_PRIM_TRIANGLE_LIST);
				updateCubeColours(colours, true);
			}
			updateCubeVertices(-2, 0, 0); // reset block (x) to middle
		}
		updateCubeVertices(0, 2, 0); // reset row (y) to middle
	}
	updateCubeVertices(0, 0, 2); // reset slice (z) to middle
}

void Renderer::updateCubeVertices(float x, float y, float z)
{
	for (int i = 0; i < CUBE_VTX_COUNT; ++i)
	{
		cubeVtx[i].x += x;
		cubeVtx[i].y += y;
		cubeVtx[i].z += z;
	}
}

void Renderer::updateCubeColours(const Cube::Colours& colours, bool revert)
{

	for (int i = 0; i < CUBE_VTX_COUNT; ++i)
	{
		Cube::Colour colour = colours[i / VTX_PER_FACE];
		cubeVtx[i].u += (revert ? -colour : colour) * 250;
	}
}

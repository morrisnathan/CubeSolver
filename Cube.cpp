#include "Cube.h"

namespace cubeSolver
{
	const Transform& Cube::getTransform() const
	{
		return transform;
	}

	const Cube::BlockColours& Cube::getColours() const
	{
		return colours;
	}

	const Vec& Cube::getXRotation() const
	{
		return xRotation;
	}
	
	const Vec& Cube::getYRotation() const
	{
		return yRotation;
	}

	const Vec& Cube::getZRotation() const
	{
		return zRotation;
	}
}

#pragma once

#include "Transform.h"

namespace cubeSolver
{
	class Cube
	{
	public:
		enum Face
		{
			front = 0,
			right,
			back,
			left,
			top,
			bottom,
			faceCount
		};

		enum Colour
		{
			blue = 0,
			orange,
			green,
			red,
			white,
			yellow,
			colourCount
		};

	private:
		Transform transform;
		Vec ltorRotation;
		Vec ftobRotation;
	};
}

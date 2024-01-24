#pragma once

namespace cubeSolver
{
	struct Vec
	{
		float x, y, z;
	};

	struct Transform
	{
		Vec translation;
		Vec rotation;
		Vec scale;
	};
}

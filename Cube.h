#pragma once

#include "Transform.h"
#include <array>

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
			none
		};

		using Colours = std::array<Colour, Face::faceCount>;
		using BlockColours = std::array<Colours, 27>;

		const Transform& getTransform() const;
		const BlockColours& getColours() const;

		const Vec& getXRotation() const;
		const Vec& getYRotation() const;
		const Vec& getZRotation() const;

	private:
		Transform transform = {
			{ 0, 0, -10 },
			{ 0.5, -0.5, 0 },
			{ 0, 0, 0 }
		};
		BlockColours colours = {
			// Front slice
			// Top row (left to right)
			std::array{ blue, none,   none,  red,  white, none },
			std::array{ blue, none,   none,  none, white, none },
			std::array{ blue, orange, none,  none, white, none },
			// Middle row
			std::array{ blue, none,   none,  red,  none,  none },
			std::array{ blue, none,   none,  none, none,  none },
			std::array{ blue, orange, none,  none, none,  none },
			// Bottom row
			std::array{ blue, none,   none,  red,  none,  yellow },
			std::array{ blue, none,   none,  none, none,  yellow },
			std::array{ blue, orange, none,  none, none,  yellow },

			// Middle slice
			// Top row
			std::array{ none, none,   none,  red,  white, none },
			std::array{ none, none,   none,  none, white, none },
			std::array{ none, orange, none,  none, white, none },
			// Middle row
			std::array{ none, none,   none,  red,  none,  none },
			std::array{ none, none,   none,  none, none,  none },
			std::array{ none, orange, none,  none, none,  none },
			// Bottom row
			std::array{ none, none,   none,  red,  none,  yellow },
			std::array{ none, none,   none,  none, none,  yellow },
			std::array{ none, orange, none,  none, none,  yellow },

			// Back slice
			// Top row
			std::array{ none, none,   green, red,  white, none },
			std::array{ none, none,   green, none, white, none },
			std::array{ none, orange, green, none, white, none },
			// Middle row
			std::array{ none, none,   green, red,  none,  none },
			std::array{ none, none,   green, none, none,  none },
			std::array{ none, orange, green, none, none,  none },
			// Bottom row
			std::array{ none, none,   green, red,  none,  yellow },
			std::array{ none, none,   green, none, none,  yellow },
			std::array{ none, orange, green, none, none,  yellow }
		};

		Vec xRotation = { 0, 0, 0 }; // left to right
		Vec yRotation = { 0, 0, 0 }; // top to bottom
		Vec zRotation = { 0, 0, 0 }; // front to back
	};
}

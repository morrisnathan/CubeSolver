#pragma once

#include "Transform.h"
#include <array>
#include <map>

#define _USE_MATH_DEFINES
#include <math.h>

namespace cubeSolver
{
	class Cube
	{
		static constexpr double FRAME_TURN_RADIANS = M_PI_2 / 20;

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

		struct SliceRotation
		{
			enum class Axis {
				x = 0, y, z, none
			};

			enum Slice {
				first = 0, second, third, all, none
			};

			Axis axis;
			Slice slice;
			double angle;

			SliceRotation() : axis(Axis::none), slice(Slice::none), angle(0) {}
		};

		bool isSliceRotating() const;
		const SliceRotation& getSliceRotation() const;

		void startRotating(SliceRotation::Axis axis, SliceRotation::Slice slice, bool clockwise);
		void rotateAndCheck();

	private:
		Transform transform = {
			{ 0, 0, -10 },
			{ 0.5, -0.5, 0 },
			{ 0, 0, 0 }
		};

		BlockColours colours = {
			// Front slice
			// Top row (left to right)
			Colours{ blue, none,   none,  red,  white, none },
			Colours{ blue, none,   none,  none, white, none },
			Colours{ blue, orange, none,  none, white, none },
			// Middle row
			Colours{ blue, none,   none,  red,  none,  none },
			Colours{ blue, none,   none,  none, none,  none },
			Colours{ blue, orange, none,  none, none,  none },
			// Bottom row
			Colours{ blue, none,   none,  red,  none,  yellow },
			Colours{ blue, none,   none,  none, none,  yellow },
			Colours{ blue, orange, none,  none, none,  yellow },

			// Middle slice
			// Top row
			Colours{ none, none,   none,  red,  white, none },
			Colours{ none, none,   none,  none, white, none },
			Colours{ none, orange, none,  none, white, none },
			// Middle row
			Colours{ none, none,   none,  red,  none,  none },
			Colours{ none, none,   none,  none, none,  none },
			Colours{ none, orange, none,  none, none,  none },
			// Bottom row
			Colours{ none, none,   none,  red,  none,  yellow },
			Colours{ none, none,   none,  none, none,  yellow },
			Colours{ none, orange, none,  none, none,  yellow },

			// Back slice
			// Top row
			Colours{ none, none,   green, red,  white, none },
			Colours{ none, none,   green, none, white, none },
			Colours{ none, orange, green, none, white, none },
			// Middle row
			Colours{ none, none,   green, red,  none,  none },
			Colours{ none, none,   green, none, none,  none },
			Colours{ none, orange, green, none, none,  none },
			// Bottom row
			Colours{ none, none,   green, red,  none,  yellow },
			Colours{ none, none,   green, none, none,  yellow },
			Colours{ none, orange, green, none, none,  yellow }
		};

		SliceRotation sliceRotation;

		const std::map<std::pair<int, int>, std::pair<int, int>> ROTATE_FORWARD_SRC_MAPPINGS = {
			{ { 0, 0 }, { 2, 0 } },
			{ { 0, 1 }, { 1, 0 } },
			{ { 0, 2 }, { 0, 0 } },
			{ { 1, 0 }, { 2, 1 } },
			{ { 1, 1 }, { 1, 1 } },
			{ { 1, 2 }, { 0, 1 } },
			{ { 2, 0 }, { 2, 2 } },
			{ { 2, 1 }, { 1, 2 } },
			{ { 2, 2 }, { 0, 2 } }
		};

		const std::map<std::pair<int, int>, std::pair<int, int>> ROTATE_BACKWARD_SRC_MAPPINGS = {
			{ { 0, 0 }, { 0, 2 } },
			{ { 0, 1 }, { 1, 2 } },
			{ { 0, 2 }, { 2, 2 } },
			{ { 1, 0 }, { 0, 1 } },
			{ { 1, 1 }, { 1, 1 } },
			{ { 1, 2 }, { 2, 1 } },
			{ { 2, 0 }, { 0, 0 } },
			{ { 2, 1 }, { 1, 0 } },
			{ { 2, 2 }, { 2, 0 } }
		};

		const std::map<Face, Face> X_ROTATE_FORWARD_MAPPINGS = {
			{ front, top },
			{ right, right },
			{ back, bottom },
			{ left, left },
			{ top, back },
			{ bottom, front }
		};

		const std::map<Face, Face> X_ROTATE_BACKWARD_MAPPINGS = {
			{ front, bottom },
			{ right, right },
			{ back, top },
			{ left, left },
			{ top, front },
			{ bottom, back }
		};

		const std::map<Face, Face> Y_ROTATE_FORWARD_MAPPINGS = {
			{ front, left },
			{ right, front },
			{ back, right },
			{ left, back },
			{ top, top },
			{ bottom, bottom }
		};

		const std::map<Face, Face> Y_ROTATE_BACKWARD_MAPPINGS = {
			{ front, right },
			{ right, back },
			{ back, left },
			{ left, front },
			{ top, top },
			{ bottom, bottom }
		};

		const std::map<Face, Face> Z_ROTATE_FORWARD_MAPPINGS = {
			{ front, front },
			{ right, bottom },
			{ back, back },
			{ left, top },
			{ top, right },
			{ bottom, left }
		};

		const std::map<Face, Face> Z_ROTATE_BACKWARD_MAPPINGS = {
			{ front, front },
			{ right, top },
			{ back, back },
			{ left, bottom },
			{ top, left },
			{ bottom, right }
		};

		Colours getNewColours(const Colours& sourceColours, const std::map<Face, Face>& rotateMappings) const;
	};
}

#include "Cube.h"
#include <map>

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

	bool Cube::isSliceRotating() const
	{
		return sliceRotation.axis != SliceRotation::Axis::none &&
			sliceRotation.slice != SliceRotation::Slice::none;
	}

	const Cube::SliceRotation& Cube::getSliceRotation() const
	{
		return sliceRotation;
	}

	void Cube::startRotating(SliceRotation::Axis axis, SliceRotation::Slice slice, bool clockwise)
	{
		sliceRotation.axis = axis;
		sliceRotation.slice = slice;
		sliceRotation.angle = (clockwise ? 0.001 : -0.001);
	}

	void Cube::rotateAndCheck()
	{
		double frameTurn = (sliceRotation.angle > 0 ? FRAME_TURN_RADIANS : -FRAME_TURN_RADIANS);
		if (sliceRotation.angle == 0.001 || sliceRotation.angle == -0.001)
			sliceRotation.angle = 0; // first time through

		sliceRotation.angle += frameTurn;
		if (sliceRotation.angle >= M_PI_2 || sliceRotation.angle <= -M_PI_2)
		{
			// Rotate block colours
			std::map<int, Colours> newColours;

			for (int z = 0; z < 3; ++z)
			{
				if (sliceRotation.axis == SliceRotation::Axis::z && z != sliceRotation.slice)
					continue;

				for (int y = 0; y < 3; ++y)
				{
					if (sliceRotation.axis == SliceRotation::Axis::y && y != sliceRotation.slice)
						continue;

					for (int x = 0; x < 3; ++x)
					{
						if (sliceRotation.axis == SliceRotation::Axis::x && x != sliceRotation.slice)
							continue;

						switch (sliceRotation.axis)
						{
						case SliceRotation::Axis::x:
						{
							const auto& source =
								sliceRotation.angle > 0 ? ROTATE_FORWARD_SRC_MAPPINGS.at({ z, y }) : ROTATE_BACKWARD_SRC_MAPPINGS.at({ z, y });
							const Colours& sourceColours = colours[source.first * 9 + source.second * 3 + x];

							newColours[z * 9 + y * 3 + x] =
								getNewColours(sourceColours, sliceRotation.angle > 0 ? X_ROTATE_FORWARD_MAPPINGS : X_ROTATE_BACKWARD_MAPPINGS);
							break;
						}
						case SliceRotation::Axis::y:
						{
							const auto& source = 
								sliceRotation.angle > 0 ? ROTATE_FORWARD_SRC_MAPPINGS.at({ z, x }) : ROTATE_BACKWARD_SRC_MAPPINGS.at({ z, x });
							const Colours& sourceColours = colours[source.first * 9 + y * 3 + source.second];

							newColours[z * 9 + y * 3 + x] =
								getNewColours(sourceColours, sliceRotation.angle > 0 ? Y_ROTATE_FORWARD_MAPPINGS : Y_ROTATE_BACKWARD_MAPPINGS);
							break;
						}
						case SliceRotation::Axis::z:
						{
							// Use the reverse mappings
							const auto& source =
								sliceRotation.angle > 0 ? ROTATE_BACKWARD_SRC_MAPPINGS.at({ y, x }) : ROTATE_FORWARD_SRC_MAPPINGS.at({ y, x });
							const Colours& sourceColours = colours[z * 9 + source.first * 3 + source.second];

							newColours[z * 9 + y * 3 + x] =
								getNewColours(sourceColours, sliceRotation.angle > 0 ? Z_ROTATE_FORWARD_MAPPINGS : Z_ROTATE_BACKWARD_MAPPINGS);
							break;
						}
						}
					}
				}
			}

			for (const auto& [index, cols] : newColours)
				colours[index] = cols;

			// Reset to not rotating
			sliceRotation.axis = SliceRotation::Axis::none;
			sliceRotation.slice = SliceRotation::Slice::none;
			sliceRotation.angle = 0;
		}
	}

	Cube::Colours Cube::getNewColours(const Colours& sourceColours, const std::map<Face, Face>& rotateMappings) const
	{
		Colours newColours;
		for (int i = 0; i < Face::faceCount; ++i)
			newColours[i] = sourceColours[rotateMappings.at((Face)i)];

		return newColours;
	}
}

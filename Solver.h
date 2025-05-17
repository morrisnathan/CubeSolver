#pragma once

#include "Cube.h"
#include <functional>

namespace cubeSolver
{
	class Solver
	{
		enum State {
			none = 0,
			yellowCentre,
			daisy,
			whiteCross
		};

		Cube& cube;
		State state;

	public:
		Solver(Cube& cube);
		void check();

	private:
		void checkYellowCentre();
		void checkDaisy();
		void checkWhiteCross();
		void solved();

		const std::map<State, std::function<void()>> stateMap = {
			{ State::none, std::bind(&Solver::checkYellowCentre, this) },
			{ State::yellowCentre, std::bind(&Solver::checkDaisy, this) },
			{ State::daisy, std::bind(&Solver::checkWhiteCross, this) },
			{ State::whiteCross, std::bind(&Solver::solved, this) }
		};
	};
}

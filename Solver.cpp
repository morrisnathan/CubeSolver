#include "Solver.h"

namespace cubeSolver
{
	Solver::Solver(Cube& cube) : cube(cube), state(none)
	{
		check();
	}

	void Solver::check()
	{
		auto checkFn = stateMap.at(state);
		checkFn();
	}

	void Solver::checkYellowCentre()
	{
	}

	void Solver::checkDaisy()
	{
	}

	void Solver::checkWhiteCross()
	{
	}

	void Solver::solved()
	{
	}
}

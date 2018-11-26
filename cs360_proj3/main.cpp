#include <iostream>

#include "PartiallyKnownGrid.h"
#include "GridPathPlanner.h"
#include "Simulation.h"

using namespace std;

int main() {
	PartiallyKnownGrid grid("map");
	
	Simulation sim (&grid);
	sim.SetStepsPerSecond(5);
	sim.SetConfirmationAfterEachMove(false);
	
	bool adaptive = true;
	bool higher_g = true;
	sim.Simulate(adaptive, higher_g);

	return 0;
}


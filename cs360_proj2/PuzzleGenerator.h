#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <math.h>
#include "Puzzle.h"
#include "Timer.h"

class PuzzleGenerator{
public:
	PuzzleGenerator(int nRows, int nColumns, int minVal, int maxVal);
	Puzzle GeneratePuzzle(); 
	Puzzle RandomWalk(double timelimit);	// Time limit given in seconds.
	Puzzle SimulatedAnnealingSearch(Puzzle initial, double startingTemp, double alpha, double stoppingTemp);

private:
	int nRows, nColumns, minVal, maxVal;
	Timer timer;
	double maxtime;
};

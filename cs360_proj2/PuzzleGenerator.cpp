#include "PuzzleGenerator.h"
#include <limits>

using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
	timer.StartTimer();
	maxtime = 58.0;	// To make sure we don't exceed a minute
	
	//double random_walk_time = 5;	// 5 seconds.
	

	/*PSUEDO FOR SIMULATED ANNEALING (Steps 2-6 done inside annealing function):  
	1. Generate random puzzle, make it the solution
	2. Calculate cost (based on sim. anneal. alg)
	3. Generate random neighboring solution
	4. Calculate that cost
	5. If new < old, make new the solution, vice versa
	6. Repeat until stopping criteria met
	7. Keep generating solutions until best value is found given time constraint
	*/

	//temperature -  initial temperature is high since we want to explore neighbors
	//alpha - rate at which temperature cools
	//stoppingTemp - the lowest temperature we hit before stopping search

	//we want to adjust temperature based on puzzle conditions, but not let it be too big
	double temperature = nRows * nColumns * ((maxVal-minVal) + 1);
	if (temperature > 250) { 
		temperature = 250;
	}
	double alpha = 0.9999; 
	double stoppingTemp = 0.001;
	//speed up the cooling by reducing alpha size if the puzzle condiitons will generate more successors
	if (nRows * nColumns >= 64 || (maxVal - minVal) >= 7) {
		alpha = 0.999; 
	}


	//generate random, make it the solution
	Puzzle initialPuzzle(nRows, nColumns, minVal, maxVal);
	Puzzle solution = SimulatedAnnealingSearch(initialPuzzle, temperature, alpha, stoppingTemp);

	//keep repeating till time is up to find best puzzle
	while (timer.GetElapsedTime() < maxtime) {

		Puzzle current = SimulatedAnnealingSearch(initialPuzzle, temperature, alpha, stoppingTemp);

		if (solution.GetValue() < current.GetValue()) {
			solution = current;
			//cout << "new solution value: " << solution.GetValue() << endl;
		}

	}

	return solution;

//*
	//return RandomWalk(random_walk_time);	// Do a random walk for some time and return the solution.
/*/
	// We could also do as many random walks as we can within the given time limit.
	Puzzle bestPuzzle = Puzzle(nRows, nColumns, minVal, maxVal);
	int bestValue = std::numeric_limits<int>::min();
	while (timer.GetElapsedTime() < maxtime-random_walk_time)
	{
		Puzzle p = RandomWalk(random_walk_time);
		int value = p.GetValue();
		if (value > bestValue) 
		{
			bestValue = value;
			bestPuzzle = p;
		}
	}
	return bestPuzzle;
//*/
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that start at a random configuration and keeps randomly modifying it
	// until t hits the time limit. Returns the best solution found so far.

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.
	
	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;			
	int bestValue = p.GetValue();
	
	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();
	
	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell 
		// (since we are doing a random walk, we just replace p with its successor).
		p = p.GetRandomSuccessor();	
		int value = p.GetValue();
		
		// Update the current best solution.
		if (value > bestValue)	// Calling GetValue() for the first time is costly
										// since the puzzle has to be evaluated first.
		{
			bestValue = value;	// Calling it a second time simply returns the value that was computed before.
			bestPuzzle = p;
		}
	}
	
	return bestPuzzle;
	
	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
	vector<Puzzle> successors;
	bestPuzzle.GetAllSuccessors(successors);
}


Puzzle PuzzleGenerator::SimulatedAnnealingSearch(Puzzle initial, double startingTemp, double alpha, double stoppingTemp) {

	/* PSUEDO FOR ACTUAL LINEAR SIM ANNEALING
	1. While stopping condition hasn't been met, generate a neighbor of current
	2. If the value of the neighor is less than current, make neighbor the best
	3. Else calculate the acceptance criteria between the two puzzles and accept new puzzle
		Acceptance criteria:
		1 - must swtich
		0 - do not switch
		0.5 - 50/50 chance
	if acceptance is greater than a random number between 0 - 1, it makes sense to switch
	4. Cool temperature based on alpha
	*/
	Puzzle current = initial;
	double temperature = startingTemp;
	Timer t;
	t.StartTimer(); //make sure search is not more than allotted time
	//while stopping condition hasn't been met keep going
	while (temperature > stoppingTemp) {

		Puzzle neighbor = current.GetRandomSuccessor();

		//if neighbor is a better puzzle
		if (neighbor.GetValue() > current.GetValue()) {

			current = neighbor;
		} 
		else {
			//ACCEPTANCE FORMULA: a = exp[(n_val - c_val) / temperature]
			double acceptance = exp((neighbor.GetValue() - current.GetValue())/temperature);
			double random = ((double)rand())/RAND_MAX; //generate value between 0 and 1
			if (acceptance > random) { //if acceptance is less than 1, accept solution
				current = neighbor;
			}

		}
		//"cool" temperature based on rate/alpha
		temperature = temperature * alpha;
	}
	return current;

}
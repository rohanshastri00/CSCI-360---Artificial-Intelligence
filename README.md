# CSCI-360---Artificial-Intelligence
My own source code for the 3 projects of the Fall 2018 semester

Topics include Propositional Logic implementation for a Clue board game simulation, Simulated Annealing for a Puzzle generator, and A*/Adaptive A* for a Puzzle solving program.

# Project 1: Uses propositional logic coupled with a Chaff algorithm solver (zchaff) to autonomously solve a game of Clue. 
# Project 2: Utilizes the local search algorithm to find the shortest path in puzzle consisting of r rows and c columns, where the integer each square in the grid denotes the number of moves the player can make in any cardinal direction.
# Project 3: Implements repeated forward A* and a variant of the A* algorithm that conducts many A* searches to update the heuristic values of the state spaces to achieve more precise searches as the current state progresses.

## To run Project 1:
Simply type "make" in terminal.

## To run Project 2:
Simply type "make" in terminal. Within the makefile, you can change the dimensions and possible values of the puzzle being generated. Note that it will take up to 58 seconds to yield a result in the terminal. 

## To run Project 3:
Type "make" in the terminal, and then "./GridPathPlanner" to run the executable.

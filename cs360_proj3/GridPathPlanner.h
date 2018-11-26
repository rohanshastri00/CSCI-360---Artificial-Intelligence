#ifndef GRID_PATH_PLANNER_H
#define GRID_PATH_PLANNER_H

#include "PartiallyKnownGrid.h"
#include <map>
#include <vector>
#include <cstddef>
#include <iostream>
#include <set>
#include <limits.h>

struct gridNode {
public:
	gridNode(xyLoc* l_) : loc(l_), f_value(INT_MAX), g_value(0), parent(NULL) {	};
	xyLoc* loc;
	int f_value;
	int g_value;
	gridNode* parent;
};


class GridPathPlanner{
public:
	GridPathPlanner(PartiallyKnownGrid* grid_, xyLoc destination_, bool adaptive_, bool larger_g_);
	~GridPathPlanner();
	
	void FindNeighbors(gridNode* current, std::vector<gridNode*>&neighbors, int** closed,std::set<gridNode*> closedNodes);
	// Finds a path from the "start" to the target (set in constructor).
	// Fills the "path" vector with a sequence of xyLocs.
	void FindPath(xyLoc start, std::vector<xyLoc> & path);
	// Return the current heuristic distance to the target.
	int GetHValue(xyLoc l);

	// Return the number of expanded nodes in the most recent search.
	int GetNumExpansionsFromLastSearch();
		
private:
	PartiallyKnownGrid* grid;	// Partially known grid (updates automatically as the agent moves.
	bool adaptive;						// If set, the heuristic should be updated after each search.
	xyLoc destination;				// Fixed goal cell.
	bool larger_g;	// If set to true, your search should tie-break towards larger g-values.
									// If set to false, your search should tie-break towards smaller g-values.
	int expansions;

	std::set<gridNode*> closedNodes;

	int** hValueTable;
};

#endif


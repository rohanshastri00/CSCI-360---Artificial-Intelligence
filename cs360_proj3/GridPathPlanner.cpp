#include "GridPathPlanner.h"
#include <limits.h>
#include <queue>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>

using namespace std;


GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid_, xyLoc destination_, bool adaptive_, bool larger_g_) {
	grid = grid_;
	destination = destination_;
	adaptive = adaptive_;
	larger_g = larger_g_;
	expansions = 0;

	hValueTable = new int*[grid->GetWidth()];

	for (int i = 0; i < grid->GetWidth();i++) {
		hValueTable[i] = new int[grid->GetHeight()];
		for(int j = 0; j < grid->GetHeight();j++) {
			hValueTable[i][j] = GetHValue(xyLoc(i, j));
		}
	}

}

GridPathPlanner::~GridPathPlanner(){
}

    
//check if neighbors are in the grid, not visited, not obstacles
void GridPathPlanner::FindNeighbors(gridNode* current, vector<gridNode*> &neighbors, int** closed, set<gridNode*> closedNodes) {
	xyLoc* uncheckedNeighbors[4];

	uncheckedNeighbors[0] = new xyLoc(current->loc->x + 1, current->loc->y); //down
	uncheckedNeighbors[1] = new xyLoc(current->loc->x - 1, current->loc->y); //up
	uncheckedNeighbors[2] = new xyLoc(current->loc->x, current->loc->y - 1); //left
	uncheckedNeighbors[3] = new xyLoc(current->loc->x, current->loc->y + 1); //right

	for (int i = 0; i < 4;i++) {
		//if neighbor isnt an obstacle, is valid and is not in my closed list
		if (!(grid->IsBlocked(*uncheckedNeighbors[i])) && 
				grid->IsValidLocation(*uncheckedNeighbors[i]) && 
					closed[uncheckedNeighbors[i]->x][uncheckedNeighbors[i]->y] == 0) {
			neighbors.push_back(new gridNode(uncheckedNeighbors[i]));
		} 

	}

	for (int i = 0; i < neighbors.size();i++) {
		neighbors[i]->parent = current;
	}
}


void GridPathPlanner::FindPath(xyLoc start, std::vector<xyLoc> & path) {

	//lamdba function to pass comparator into priority queue, need correct larger g boolean value
	auto compare = [this] (const gridNode* p1, const gridNode* p2)
    { 
    	if (p1->f_value == p2->f_value) {
        	if (larger_g) {
        		return p1->g_value <= p2->g_value;
        	} else {
        		return p1->g_value > p2->g_value;
       		} 
    	}
    	return p1->f_value > p2->f_value;
    };

	//pq for open list (to get min f value)
	priority_queue <gridNode*, vector<gridNode*>, function<bool(const gridNode*, const gridNode*)> > open(compare); 
	set<gridNode*> closedNodes;

	int width = grid->GetWidth();
	int height = grid->GetHeight();
	int** closed = new int*[width];
	for (int i = 0; i < width;i++) {
		closed[i] = new int[height];
		for (int j = 0; j < height;j++) {
			closed[i][j] = 0; //default empty
		}
	}

	expansions = 0;

	gridNode* temp = new gridNode(new xyLoc(start.x, start.y));
	open.push(temp);

	gridNode* dest = new gridNode(new xyLoc(destination.x, destination.y));


	while (!(open.empty())) {
		//top node which has lowest fScore value (comparator)
		gridNode* current = open.top();

		if (*(current->loc) == *(dest->loc)) {
			int dest_g = current->g_value;
			path.push_back(*(current->loc));

			while (current->parent != NULL) {
				path.push_back(*(current->loc));
				current = current->parent;
			}
			path.push_back(start);
			reverse(path.begin(), path.end());

			if (adaptive == true) {
				//iterate through closed list, update h values
				set<gridNode*>::iterator it = closedNodes.begin();
				while (it != closedNodes.end())
				{
	   				//update table
	   				hValueTable[(*it)->loc->x][(*it)->loc->y] = dest_g - (*it)->g_value;
   					it++;
				}
			}
			return;
		}

		open.pop(); //remove top element

		//if current is already in the closed list, forget it move on
		if (closed[current->loc->x][current->loc->y] == 1) {
			continue;
		}
		expansions++;

		closed[current->loc->x][current->loc->y] = 1;
		closedNodes.insert(current);
		//initially, current will be the start node, then will be node with lowest f value
		vector<gridNode*> neighbor_arr;
		FindNeighbors(current, neighbor_arr, closed, closedNodes);

		for (int i = 0; i < neighbor_arr.size();i++) {

			int tentative_gScore = current->g_value + 1;

			neighbor_arr[i]->g_value = tentative_gScore;
 			neighbor_arr[i]->f_value = neighbor_arr[i]->g_value + hValueTable[neighbor_arr[i]->loc->x][neighbor_arr[i]->loc->y];
			open.push(neighbor_arr[i]);
		}
	}

}


int GridPathPlanner::GetHValue(xyLoc l) {
	int manhattan_value = abs(l.x - destination.x) + abs(l.y - destination.y);
	return manhattan_value;
}

int GridPathPlanner::GetNumExpansionsFromLastSearch() {
	return expansions;
}

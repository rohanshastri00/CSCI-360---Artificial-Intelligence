#include <iostream>
#include "PartiallyKnownGrid.h"
#include "Timer.h"

class Simulation {
	public:
		Simulation(PartiallyKnownGrid* grid) {
			grid_ = grid;
			wait_time_ = 100;
			wait_for_confirmation_ = false;
		}

		~Simulation(){}

		void SetStepsPerSecond(double n) {
			wait_time_ = 1000.0/n;
		}
		void SetConfirmationAfterEachMove(bool confirmation) {
			wait_for_confirmation_ = confirmation;
		}
		
		void Simulate(bool adaptive, bool larger_g)	{
			GridPathPlanner planner(grid_, grid_->GetGoalLocation(), adaptive, larger_g);
			Timer t;

			// Start simulation
			int steps = 0;
			int total_expansions = 0;
			double total_time = 0;
			grid_->Reset();
			grid_->DrawGrid();

			while (!grid_->GoalReached()) {  // loop until the robot find the target or dies
			
					bool clear_terminal = true;
					if (clear_terminal) {
					#if defined(_WIN32) || defined(_WIN64)
		        system("cls");
					#else
        	system("clear");
						#endif
					}
					
					
			    grid_->DrawGrid();
			    if (adaptive)
			    	std::cout<<"Adaptive A*, ";
			    else
			    	std::cout<<"Forward A*, ";

			    if (larger_g)
			    	std::cout<<"large g";
			    else
			    	std::cout<<"small g";
			    std::cout<<std::endl;
			    	
			    std::cout<<"Current location: "<<grid_->GetCurrentLocation()<<std::endl;
			    std::cout<<"Heuristic distance to destination: "<<planner.GetHValue(grid_->GetCurrentLocation())<<std::endl;
					
					t.StartTimer();
					std::vector<xyLoc> path;
					planner.FindPath(grid_->GetCurrentLocation(), path);

					double time = t.GetElapsedTime();
					total_time += time;
					int expansions = planner.GetNumExpansionsFromLastSearch();
					total_expansions += expansions;
										
					// Verify path.
					if (!ValidatePath(grid_->GetCurrentLocation(), grid_->GetGoalLocation(), path)){
			    	std::cout<<"Stopping simulation due to invalid path..."<<std::endl;
			    	return;		
					}

					std::cout<<std::endl;
					
					std::cout<<"Search statistics"<<std::endl;
					std::cout<<"-----------------"<<std::endl;
			    std::cout<<"Path length to destination: "<<path.size()-1<<std::endl;
			    std::cout<<"Number of expansions: "<<expansions<<std::endl;
			    std::cout<<"Search (and update) time (ms): "<<1000*time<<std::endl;
			    if (adaptive)
				    std::cout<<"Updated heuristic distance to destination: "<<planner.GetHValue(grid_->GetCurrentLocation())<<std::endl;
		    			    
					xyLoc move_to = path[1];
					steps++;
					std::cout<<"Step "<<steps<<": "<<grid_->GetCurrentLocation()<<" -> "<<move_to<<std::endl;	
							
			    // Call the simulator to move your robot and count the steps
			    bool valid_move = grid_->MoveTo(move_to);
			    if (!valid_move) {
			    	std::cout<<"Stopping simulation due to invalid move..."<<std::endl;
			    	return;
			    }
									

					if (wait_for_confirmation_)
						while (std::cin.get() != '\n');	

			    // TODO: Wait for console input

			    #if defined(_WIN32) || defined(_WIN64)
			    Sleep(wait_time_);
			    #else
			    //*
			    struct timespec req, rem;
			    req.tv_nsec = 1000000*wait_time_;
			    req.tv_sec = 0;
			    nanosleep(&req, &rem);
			    /*/
					usleep(1000*wait_time_);
					//*/
				    #endif
				}
			std::cout<<"Target found in "<<steps<<" steps !!!"<<std::endl;
	    std::cout<<"Average number of expansions: "<<total_expansions/(double)steps<<std::endl;
	    std::cout<<"Average search (and update) time (ms): "<<1000*total_time/(double)steps<<std::endl;
		}

	private:
		PartiallyKnownGrid* grid_;
		bool adaptive_;	
		double wait_time_;
		bool wait_for_confirmation_;
		
		bool ValidatePath(xyLoc s, xyLoc t, std::vector<xyLoc> & path) {
		  if (path.size() == 0) {
				std::cout<<"Path is empty!"<<std::endl;
				return false;
		  }
		  if (!(s == path.front())) {
 				std::cout<<"The path should start at  "<<s<<"!"<<std::endl;
		    return false;
		  }
		  if (!(t == path.back())) {
 				std::cout<<"The path should end at "<<t<<"!"<<std::endl;
		    return false;
		  }

		  for (int i = 0; i < path.size()-1; i++) {
		  	if (!grid_->IsValidLocation(path[i+1])) {
	 				std::cout<<"Cell "<<path[i+1]<<" on the path is out of bounds!"<<std::endl;
			    return false;		  	
		  	}
		  	
				if (grid_->IsBlocked(path[i+1])) {
					std::cout<<"Cell "<<path[i+1]<<" on the path is blocked!"<<std::endl;
			    return false;		  	
				}

		    if (!(path[i] == path[i+1])) {
	    		int dx = abs(path[i].x - path[i+1].x);
					int dy = abs(path[i].y - path[i+1].y);
					if (dx + dy > 1) {
			 			std::cout<<"Invalid move on path: "<<s<<" to "<<t<<"!"<<std::endl;
			 			return false;
			 		}
		    }
		  }
		  return true;
		}
};


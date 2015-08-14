#pragma once
#include <random>
#include <algorithm>
#include <vector>

using namespace std ;
enum Transition {LEFT, STAY, RIGHT} ;

class Cell
{
	public:
		Cell() {}
		~Cell() {}
		
		// Initialise domain with empty cells
		void SetNumCells(int numCells){
			occupied.clear() ;
			for (int i = 0; i < numCells; i++)
				occupied.push_back(false) ;
		}
		
		// Change the occupancy state of the cell
		void ToggleOccupied(int State){
			occupied[State] = !occupied[State] ;
		}
		
		vector<bool> occupied ;
} ;

class Target
{
	public:
		Target(int state, int eState): numActions(3), itsState(state), itsEnergy(eState) {}
		
		~Target() {}
		
		// Perform target state transition
		void TargetTransition(Cell & AllCells){
			int temp = rand() % numActions ;
	
			// Only make transition if transition cell is not occupied
			int newState ;
			switch (temp){
				case 0:
					itsAction = LEFT ;
					newState = max(0,itsState-1) ;
					if (!AllCells.occupied[newState]){ // make transition
						AllCells.ToggleOccupied(itsState) ;
						itsState = newState ;
						AllCells.ToggleOccupied(itsState) ;
					}
					break ;
				case 1:
					itsAction = STAY ;
					break ;
				case 2:
					itsAction = RIGHT ;
					newState = min(itsState+1,(int)AllCells.occupied.size()-1) ;
					if (!AllCells.occupied[newState]){ // make transition
						AllCells.ToggleOccupied(itsState) ;
						itsState = newState ;
						AllCells.ToggleOccupied(itsState) ;
					}
					break ;
			}
		}
		
		void ReduceEnergy() {itsEnergy-- ;}
		int GetState() {return itsState ;}
		int GetEnergy() {return itsEnergy ;}
	private:
		int numActions ;
		int itsState ;
		int itsEnergy ;
		vector<double> itsTransition ;
		Transition itsAction ;
} ;

//void Target::TargetTransition(Cell & AllCells){
//	default_random_engine generator;
//	uniform_real_distribution<double> distribution(1,100);
//	double pTrans = distribution(generator) / 100.0 ;
//	int temp ;
//	for (int i = 0; i < cdfActions; i++){
//		if (pTrans <= itsTransition[i]){
//			temp = i ;
//			break ;
//		}
//	}
//	
//	// Only make transition if transition cell is not occupied
//	int newState ;
//	switch (temp){
//		case 0:
//			itsAction = LEFT ;
//			newState = min(0,itsState-1) ;
//			if (!AllCells.occupied[newState]){ // make transition
//				AllCells.ToggleOccupied(itsState) ;
//				itsState = newState ;
//				AllCells.ToggleOccupied(itsState) ;
//			}
//			break ;
//		case 1:
//			itsAction = STAY ;
//			break ;
//		case 2:
//			itsAction = RIGHT ;
//			newState = max(itsState+1,(int)AllCells.occupied.size()) ;
//			if (!AllCells.occupied[newState]){ // make transition
//				AllCells.ToggleOccupied(itsState) ;
//				itsState = newState ;
//				AllCells.ToggleOccupied(itsState) ;
//			}
//			break ;
//	}	
//}

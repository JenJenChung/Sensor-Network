#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>
#include <random>
#include <fstream>
#include <sstream>
#include "Target.h"
#include "Sensor.h"

using namespace std ; 

class SensorNetwork
{
	public:
		SensorNetwork(): numTargets(2), totalTargets(2), targetEFull(3), numCells(3) {
			CalculateAllStates() ;
			InitialiseTargets() ;
			InitialiseSensors() ;
		}
		
		SensorNetwork(int targets, int efull, int cells): numTargets(targets), totalTargets(targets), targetEFull(efull), numCells(cells){
			if (numTargets > numCells){
				cout << "warning: number of targets must be less than or equal to number of cells. "
				<< "Reducing number of targets to number of cells.\n" ;
				numTargets = numCells ;
			}
			CalculateAllStates() ;
			InitialiseTargets() ;
			InitialiseSensors() ;
		}
		
		~SensorNetwork() {}
		
		int GetStateID(vector<int> state){
			for (unsigned i = 0; i < allStates.size(); i++){
				if (VectorComparison(allStates[i],state))
					return i ;
			}
			cout << "Error: invalid state!\n" ;
			return -1 ;
		}
		
		int GetStateID(){
			vector<int> state(numCells,0) ;
			for (int i = 0; i < numTargets; i++)
				state[allTargets[i].GetState()] = allTargets[i].GetEnergy() ;
			return GetStateID(state) ;
		}
		
		vector< vector<int> > GetAllStates() const {return allStates ;}
		
		void Iterate(string fileName){
			ComputeGlobalReward() ;
//			cout << "Global reward: " << globalReward ;
			StateTransition() ;
//			cout << ". Next state: " << GetStateID() << endl ;
			int newState = GetStateID() ;
			for (unsigned i = 0; i < allSensors.size(); i++){
				allSensors[i].SetReward(globalReward) ;
			}
			LogData(fileName) ;
			itsStateID = newState ;
			for (unsigned i = 0; i < allSensors.size(); i++){
				allSensors[i].ChooseAction(itsStateID, newState) ;
			}
		}
		
		void ResetTargets(){
			InitialiseTargets() ;
		}
		
	private:
		int numTargets ;
		int totalTargets ;
		int targetEFull ;
		int numCells ;
		vector< vector<int> > allStates ;
		Cell currentOccupation ;
		int itsStateID ;
		vector<Target> allTargets ;
		vector<Sensor> allSensors ;
		vector<int> jointAction ;
		int globalReward ;
		ofstream logFile ;
		
		bool VectorComparison(vector<int> u, vector<int> v) ;
		void CalculateAllStates() ;
		void InitialiseTargets() ;
		void InitialiseSensors() ;
		unsigned nChoosek( unsigned n, unsigned k ) ;
		void ComputeGlobalReward() ;
		void StateTransition() ;
		void LogData(string fileName) ;
} ;

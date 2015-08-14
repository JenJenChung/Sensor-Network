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
		SensorNetwork(): numTargets(2), totalTargets(2), targetEFull(3), numCells(3), isRanged(false) {
			CalculateAllStates() ;
			InitialiseTargets() ;
			InitialiseSensors() ;
		}
		
		SensorNetwork(int targets, int efull, int cells): 
		numTargets(targets), totalTargets(targets), targetEFull(efull), numCells(cells), isRanged(false){
			// Check for default value tags
			if (targets < 0){
				numTargets = 2 ;
				totalTargets = 2 ;
			}
			if (efull < 0)
				targetEFull = 3 ;
			if (cells < 0)
				numCells = 3 ;
			
			if (numTargets > numCells){
				cout << "warning: number of targets must be less than or equal to number of cells.\n" ;
				cout << "Reducing number of targets to number of cells.\n" ;
				numTargets = numCells ;
				totalTargets = numCells ;
			}
			
			// Initialise domain and sensor agents
			CalculateAllStates() ;
			InitialiseTargets() ;
			InitialiseSensors() ;
		}
		
		SensorNetwork(int targets, int efull, int cells, int range): 
		numTargets(targets), totalTargets(targets), targetEFull(efull), numCells(cells), isRanged(true){
			// Check for default value tags
			if (targets < 0){
				numTargets = 2 ;
				totalTargets = 2 ;
			}
			if (efull < 0)
				targetEFull = 3 ;
			if (cells < 0)
				numCells = 3 ;
			
			if (numTargets > numCells){
				cout << "warning: number of targets must be less than or equal to number of cells.\n" ;
				cout << "Reducing number of targets to number of cells.\n" ;
				numTargets = numCells ;
				totalTargets = numCells ;
			}
			
			// Initialise domain and sensor agents
			CalculateAllStates() ;
			InitialiseTargets() ;
			if (range < 0){
				isRanged = false ;
				InitialiseSensors() ;
			}
			else
				InitialiseSensors(range) ;
		}
		
		~SensorNetwork() {}
		
		// Assign learning algorithm for all sensor agents
		void SetLearningAlgorithm(LearningType algorithm){
			for (unsigned i = 0; i < allSensors.size(); i++)
				allSensors[i].SetLearning(algorithm) ;
		}
		
		// Assign sensor observability range for all agents
		void SetSensorRange(int range){
			for (unsigned i = 0; i < allSensors.size(); i++)
				allSensors[i].SetRange(range, allStates) ;
		}
		
		// Identify global state ID of input state vector
		int GetStateID(vector<int> state){
			for (unsigned i = 0; i < allStates.size(); i++){
				if (VectorComparison(allStates[i],state))
					return i ;
			}
			cout << "Error: invalid state!\n" ;
			return -1 ;
		}
		
		// Identify global state ID of current state
		int GetStateID(){
			vector<int> state(numCells,0) ;
			for (int i = 0; i < numTargets; i++)
				state[allTargets[i].GetState()] = allTargets[i].GetEnergy() ;
			return GetStateID(state) ;
		}
		
		vector< vector<int> > GetAllStates() const {return allStates ;}
		
		// Main learning iteration
		void Iterate(string fileName){
			ComputeGlobalReward() ;
			StateTransition() ;
			int newState = GetStateID() ;
			
			// Transmit global reward to all sensor agents
			for (unsigned i = 0; i < allSensors.size(); i++){
				allSensors[i].SetReward(globalReward) ;
			}
			
			// Log learning step
			LogData(fileName) ;
			
			// Choose next action
			for (unsigned i = 0; i < allSensors.size(); i++){
				if (isRanged) // decentralised
					allSensors[i].ChooseAction(allStates[itsStateID], allStates[newState]) ;
				else // centralised
					allSensors[i].ChooseAction(itsStateID, newState) ;
			}
			
			itsStateID = newState ;
		}
		
		// Reset target energy to full and target location to random
		void ResetTargets(){
			InitialiseTargets() ;
		}
		
		// Reset state-action trace to zeros
		void ResetActionTrace(){
			for (unsigned i = 0; i < allSensors.size(); i++)
				allSensors[i].ResetTrace() ;
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
		bool isRanged ;
		
		bool VectorComparison(vector<int> u, vector<int> v) ;
		void CalculateAllStates() ;
		void InitialiseTargets() ;
		void InitialiseSensors() ;
		void InitialiseSensors(int range) ;
		unsigned nChoosek( unsigned n, unsigned k ) ;
		void ComputeGlobalReward() ;
		void StateTransition() ;
		void LogData(string fileName) ;
} ;

// Test sensor network files
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "SensorNetwork.h"

// LearningType {QLEARNING, SARSA, SARSALAMBDA} ;

int main()
{
	// Domain parameters, note: any negative inputs will apply the default values
	int numTargets = 3 ;		// number of targets, default 2
	int fullEnergy = 5 ;		// initial target energy, default 3
	int numCells = 5 ;			// number of cells, default 3
	int obsRange = 1 ;			// number of cells on either side of sensor that are observable, default full
	
	SensorNetwork * testNetwork = new SensorNetwork(numTargets, fullEnergy, numCells, obsRange) ;
	LearningType algorithm = QLEARNING ;
	testNetwork->SetLearningAlgorithm(algorithm) ;
	
	vector< vector<int> > allStates = testNetwork->GetAllStates() ;
	
	// Write states to file
	ofstream statesFile ;
	statesFile.open("states.txt") ;
	for (unsigned i = 0; i < allStates.size(); i++)
	{
		for (unsigned j = 0; j < allStates[i].size(); j++)
		{
			statesFile << allStates[i][j] ;
			if (j == allStates[i].size()-1)
				statesFile << "\n" ;
			else
				statesFile << "," ;
		}
	}
	statesFile.close() ;
	
	// Initialise log file
	std::string logFileName = "log.txt" ;
	ofstream logFile ;
	
	// Iterate through each episode
	unsigned k = 0 ;
	unsigned nEps = 100 ;
	while (k < nEps){
		logFile.open(logFileName,ios_base::app) ;
		logFile << "Episode " << k << endl ;
		logFile.close() ;
		
		cout << "Episode " << k << "..." ;
		
		// Iterate for 1000 steps or until all targets are eliminated
		unsigned i = 0 ;
		while (i < 1000){
			testNetwork->Iterate(logFileName) ;
			if (testNetwork->GetStateID() == 0)
				break ;
			i++ ;
		}
		
		cout << i << " steps.\n" ;
		 
		testNetwork->ResetTargets() ;
		k++ ;
	}
	
	return 0 ;
}

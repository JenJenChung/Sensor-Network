// Test sensor network files
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "SensorNetwork.h"

int main()
{
	// Default is 2 targets, 3 energy, 3 cells
	SensorNetwork * testNetwork = new SensorNetwork(4,10,6) ;
	
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
	
	cout << "Initialised state ID: " << testNetwork->GetStateID() << endl ;
	
	unsigned i = 0 ;
	while (i < 1000){
		testNetwork->Iterate() ;
		if (testNetwork->GetStateID() == 0)
			break ;
		i++ ;
	}
	
	return 0 ;
}

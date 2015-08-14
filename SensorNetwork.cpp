#include "SensorNetwork.h"

// Return true if input vectors have identical elements, false otherwise
bool SensorNetwork::VectorComparison(vector<int> u, vector<int> v){
	bool eq = true ;
	for (unsigned i = 0; i < u.size(); i++){
		if (u[i] != v[i]){
			eq = false ;
			break ;
		}
	}
	return eq ;
}

// Compute and record complete discrete state space
void SensorNetwork::CalculateAllStates(){
	vector<int> cells(numCells,0) ;
	vector< vector<int> > tempStates((int)pow(2,numCells),cells) ;
	vector<bool> occupied(numCells, false) ;
	vector< vector<int> > baseStates ;
	
	for (unsigned i = 0; i < tempStates.size(); i++){
		int sumCells = 0 ;
		for (unsigned j = 0; j < occupied.size(); j++){
			if (occupied[j])
				tempStates[i][j] = 1 ;
			else
				tempStates[i][j] = 0 ;
			if (i%((int)pow(2,j)) == 0)
				occupied[j] = !occupied[j] ;
			sumCells += tempStates[i][j] ;
		}
		if (sumCells <= numTargets)
			baseStates.push_back(tempStates[i]) ;
	}
	
	// Set up all possible permutations
	vector<int> targets(numTargets,0) ;
	vector< vector<int> > eStates((int)pow(targetEFull,numTargets),targets) ;
	vector<int> energy(numTargets,targetEFull) ;
	for (unsigned i = 0; i < eStates.size(); i++){
		for (int j = 0; j < numTargets; j++){
			eStates[i][j] = energy[j] ;
			if ((i+1)%((int)pow(targetEFull,j+1)) == 0)
				energy[j] = targetEFull ;
			else if ((i+1)%((int)pow(targetEFull,j)) == 0)
				energy[j]-- ;
		}
	}
	
	// Assign all valid permutations to base states
	int numBaseStates = baseStates.size() ;
	for (int i = 0; i < numBaseStates; i++){
		int sumCells = 0 ;
		for (int j = 0; j < numCells ; j++)
			sumCells += baseStates[i][j] ;
		for (int j = 0; j < (int)pow(targetEFull,sumCells); j++){
			int ii = 0 ;
			for (int k = 0; k < numCells; k++){
				if (baseStates[i][k] != 0){
					baseStates[i][k] = eStates[j][ii] ;
					ii++ ;
				}
			}
			allStates.push_back(baseStates[i]) ;
		}
	}
}

// Set random target configuration with full energy
void SensorNetwork::InitialiseTargets(){
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() ;
	default_random_engine generator(seed) ;
	uniform_int_distribution<int> distribution(1,numCells) ;
	currentOccupation.SetNumCells(numCells) ;
	allTargets.clear() ;
	for (int i = 0; i < totalTargets; i++){
		while (true){
			int cellID = distribution(generator) - 1 ;
			if (!currentOccupation.occupied[cellID]){
				Target newTarget(cellID, targetEFull) ;
				allTargets.push_back(newTarget) ;
				currentOccupation.occupied[cellID] = true ;
				break ;
			}
		}
	}
	itsStateID = GetStateID() ;
}

// Centralised sensor network share the global state vector
void SensorNetwork::InitialiseSensors(){
	int numSensors = 2*(numCells + 1) ;
	for (int i = 0; i < numSensors; i++){
		if (i/2 == 0){
			Sensor newSensor(0,0) ;
			newSensor.InitialisePolicy(allStates.size()) ;
			allSensors.push_back(newSensor) ;
		}
		else if (i/2 == numCells){
			Sensor newSensor(numCells-1,numCells-1) ;
			newSensor.InitialisePolicy(allStates.size()) ;
			allSensors.push_back(newSensor) ;
		}
		else {
			Sensor newSensor(i/2-1,(i/2)) ;
			newSensor.InitialisePolicy(allStates.size()) ;
			allSensors.push_back(newSensor) ;
		}
	}
//	// Write sensors to file
//	ofstream sensorsFile ;
//	sensorsFile.open("sensors.txt") ;
//	for (unsigned i = 0; i < allSensors.size(); i++)
//		sensorsFile << allSensors[i].GetLeft() << "," << allSensors[i].GetRight() << "\n" ;
//	sensorsFile.close() ;
}

// Decentralised sensor network with local state vectors
void SensorNetwork::InitialiseSensors(int range){
	int numSensors = 2*(numCells + 1) ;
	for (int i = 0; i < numSensors; i++){
		if (i/2 == 0){
			Sensor newSensor(0,0) ;
			newSensor.SetRange(range, allStates) ;
			newSensor.InitialisePolicy() ;
			allSensors.push_back(newSensor) ;
		}
		else if (i/2 == numCells){
			Sensor newSensor(numCells-1,numCells-1) ;
			newSensor.SetRange(range, allStates) ;
			newSensor.InitialisePolicy() ;
			allSensors.push_back(newSensor) ;
		}
		else {
			Sensor newSensor(i/2-1,(i/2)) ;
			newSensor.SetRange(range, allStates) ;
			newSensor.InitialisePolicy() ;
			allSensors.push_back(newSensor) ;
		}
	}
}

// Compute combinatorial for determining state space size
unsigned SensorNetwork::nChoosek( unsigned n, unsigned k )
{
    if (k > n) return 0;
    if (k * 2 > n) /*return*/ k = n-k;  //remove the commented section
    if (k == 0) return 1;
 
    int result = n;
    for( unsigned i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

// Compute global reward: -1 for each sensing action, +30 for each removed target
void SensorNetwork::ComputeGlobalReward()
{
	vector<int> tracked(numCells,0) ;
	globalReward = 0 ;
	for (unsigned i = 0; i < allSensors.size(); i++){
		switch (allSensors[i].GetAction()) {
			case TRACKLEFT:
				if (allSensors[i].GetRight() != 0)
					tracked[allSensors[i].GetLeft()]++ ;
				globalReward-- ;
				break ;
			case NOTRACK:
				break ;
			case TRACKRIGHT:
				if (allSensors[i].GetLeft() != numCells)
					tracked[allSensors[i].GetRight()]++ ;
				globalReward-- ;
				break ;
		}
	}
	
	// Identify potential hits
	jointAction.clear() ;
	vector<bool> effectiveAction ;
	for (unsigned i = 0; i < tracked.size(); i++){
		if (tracked[i] >= 3)
			effectiveAction.push_back(true) ;
		else
			effectiveAction.push_back(false) ;
		jointAction.push_back(tracked[i]) ;
	}
	
	// Reduce energy state of targets that have been hit
	vector<Target> temp ;
	for (unsigned i = 0; i < allTargets.size(); i++) {
		if (effectiveAction[allTargets[i].GetState()])
			allTargets[i].ReduceEnergy() ;
		if (allTargets[i].GetEnergy() > 0)
			temp.push_back(allTargets[i]) ;
	}
	
	// Identify and remove 0 energy targets
	if (allTargets.size() > temp.size())
		globalReward += 30*(allTargets.size()-temp.size()) ;
	allTargets.clear() ;
	for (unsigned i = 0; i < temp.size(); i++)
		allTargets.push_back(temp[i]) ;
	numTargets = allTargets.size() ;
}

// Initiate target transition
void SensorNetwork::StateTransition()
{
	for (int i = 0; i < numTargets; i++)
		if (allTargets[i].GetEnergy() != 0)
			allTargets[i].TargetTransition(currentOccupation) ;
}

// Log learning step data
void SensorNetwork::LogData(string fileName)
{
	logFile.open(fileName,ios_base::app) ;
	logFile << itsStateID << "," ;
	for (unsigned i = 0; i < jointAction.size(); i++)
	{
		logFile << jointAction[i]  << "," ;
	}
	logFile << globalReward << "\n" ;
	logFile.close() ;
} 

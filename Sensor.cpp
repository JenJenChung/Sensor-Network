#include "Sensor.h"

int Sensor::ObserveLocalState(vector<int> globalState){
	int numCells = globalState.size() ;
	int leftLim = itsLeft - itsRange + 1 ;
	int rightLim = itsRight + itsRange - 1 ;
	vector<int> temp ;
	for (int i = rightLim; i <= leftLim; i++){
		if (i < 0 || i >= numCells)
			temp.push_back(0) ;
		else
			temp.push_back(globalState[i]) ;
	}
	
	for (int i = 0; i < itsState.size(); i++)
		if (VectorCompare(temp, itsState[i]))
			return i ;
	
	cout << "Error: invalid state!\n" ;
	return -1 ;
}

bool Sensor::VectorCompare(vector<int> u, vector<int> v){
	bool eq = true ;
	for (unsigned i = 0; i < u.size(); i++){
		if (u[i] != v[i]){
			eq = false ;
			break ;
		}
	}
	return eq ;
}

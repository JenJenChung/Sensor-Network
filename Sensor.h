#pragma once
#include <iostream>
#include <stdlib.h>
#include "Policy.h"

enum SensorAction {TRACKLEFT, NOTRACK, TRACKRIGHT} ; 

class Sensor
{
	public:
		Sensor(int left, int right): leftCell(left), rightCell(right), numActions(3) {
			int a0 = rand() % numActions ;
			switch (a0) {
				case 0:
					itsAction = TRACKLEFT ;
					break ;
				case 1:
					itsAction = NOTRACK ;
					break ;
				case 2:
					itsAction = TRACKRIGHT ;
					break ;
			}
		}
		~Sensor() {}
		
		int GetAction() const {return itsAction ;}
		int GetLeft() const {return leftCell ;}
		int GetRight() const {return rightCell ;}
		
		void SetLearning(LearningType algorithm){
			itsPolicy.SetLearningType(algorithm) ;
		}
		
		void SetRange(int range, vector< vector<int> > allStates){
			itsRange = range ;
			vector<int> temp ;
			for (unsigned i = 0; i < allStates.size(); i++){
				temp.clear() ;
				for (int j = 0; j < range*2; j++)
					temp.push_back(allStates[i][j]) ;
				
				bool newState = true ;
				for (unsigned j = 0; j < itsStates.size(); j++)
					if (VectorCompare(temp,itsStates[j]))
						newState = false ;
				if (newState)
					itsStates.push_back(temp) ;
			}
		}
		
		void InitialisePolicy(){
			itsPolicy.SetQ(numStates, numActions) ;
		}
		
		void InitialisePolicy(int nStates){
			itsPolicy.SetQ(nStates, numActions) ;
		}
		
		void ResetTrace(){
			itsPolicy.SetTrace() ;
		}
		
		void SetReward(int reward) {itsReward = reward ;}
		
		void ChooseAction(vector<int> gState0, vector<int> gState1){
			int state0 = ObserveLocalState(gState0) ;
			int state1 = ObserveLocalState(gState1) ;
			int newAction = itsPolicy.NextAction(state0, itsAction, itsReward, state1) ;
			switch (newAction){
				case 0:
					itsAction = TRACKLEFT ;
					break ;
				case 1:
					itsAction = NOTRACK ;
					break ;
				case 2:
					itsAction = TRACKRIGHT ;
					break ;
			}
		}
		
		void ChooseAction(int state0, int state1){
			int newAction = itsPolicy.NextAction(state0, itsAction, itsReward, state1) ;
			switch (newAction){
				case 0:
					itsAction = TRACKLEFT ;
					break ;
				case 1:
					itsAction = NOTRACK ;
					break ;
				case 2:
					itsAction = TRACKRIGHT ;
					break ;
			}
		}
		
	private:
		int leftCell ;
		int rightCell ;
		int numActions ;
		SensorAction itsAction ;
		int itsReward ;
		Policy itsPolicy ;
		int itsRange ;
		vector< vector<int> > itsStates ;
		int numStates ;
		
		int ObserveLocalState(vector<int> globalState) ;
		bool VectorCompare(vector<int> u, vector<int> v) ;
} ;

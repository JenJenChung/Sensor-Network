#pragma once
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
		
		void InitialisePolicy(int numStates){
			itsPolicy.SetQ(numStates, numActions) ;
		}
		
		void SetReward(int reward) {itsReward = reward ;}
		
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
} ;

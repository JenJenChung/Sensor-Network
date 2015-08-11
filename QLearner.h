#pragma once
#include <vector>

class Sensor ;

class QLearner
{
	public:
		enum Reward {G, D} ;
		QLearner(): alpha(0.9), gamma(0.9), rType(G){
			
		}
		~QLearner() {}
		
		void SetG(int G) {itsG = G ;}
		void SetD(int D) {itsD = D ;}
		
		void ValueIteration(SensorAction action, int greward, int dreward){
			SetG(greward) ;
			SetD(dreward) ;
			SetReward() ;
			itsQ[action] = itsQ[action] + alpha*((double)itsReward + gamma*GetMaxQ() - itsQ[action]) ;
		}
		
		int GetBestAction(){
			int maxA = 0 ;
			double maxQ = itsQ[0] ;
			for (int i = 0; i < itsQ.size(); i++){
				if (itsQ[i] > maxQ){
					maxA = i ;
					maxQ = itsQ[i] ;
				}
			}
			return maxA ;
		}
		
		void SetReward(){
			switch (rType) {
				case: G
					itsReward = itsG ;
					break ;
				case: D
					itsReward = itsD ;
					break ;
			}
		}
	private:
		vector<double> itsQ ;
		double alpha ;
		double gamma ;
		int itsG ;
		int itsD ;
		int itsReward ;
		Reward rType ;
		
		double GetMaxQ(){
			double maxQ = itsQ[0] ;
			for (int i = 0; i < itsQ.size(); i++){
				if (itsQ[i] > maxQ)
					maxQ = itsQ[i] ;
			}
			return maxQ ;
		}
} ;

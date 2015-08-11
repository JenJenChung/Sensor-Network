#pragma once
#include <vector>
#include <random>
#include <stdlib.h>
#include <chrono>

class Policy
{
	public:
		Policy(): alpha(0.9), gamma(0.9), eps(0.1) {}
		~Policy() {}
		
		void SetQ(int numStates, int numActions){
			vector< double > temp(numActions,0.0) ;
			for (int i = 0; i < numStates; i++)
				itsQ.push_back(temp) ;
		}
		
		int NextAction(int state0, int action, int reward, int state1){
			itsReward = reward ;
			itsQ[state0][action] = itsQ[state0][action] + alpha*(itsReward + gamma*GetMaxQ(state1) - itsQ[state0][action]) ;
			
			int tempMaxQ = itsQ[state1][0] ;
			int tempAction = 0 ;
			for (unsigned i = 0; i < itsQ[state1].size(); i++){
				if (itsQ[state1][i] > tempMaxQ){
					tempMaxQ = itsQ[state1][i] ;
					tempAction = i ;
				}
			}
			
			int newAction ;
			int numActions = itsQ[0].size() ;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() ;
			default_random_engine generator(seed);
			uniform_real_distribution<double> distribution(0.0,0.1);
			double pAct = distribution(generator) ;
			if (pAct <= eps)
				newAction = rand() % numActions ;
			else
				newAction = tempAction ;
			
			return newAction ;
		}
	private:
		vector< vector<double> > itsQ ;
		double alpha ;
		double gamma ;
		double eps ;
		double itsReward ;
		
		double GetMaxQ(int state){
			double maxQ = itsQ[state][0] ;
			for (unsigned i = 0; i < itsQ[state].size(); i++){
				if (itsQ[state][i] > maxQ)
					maxQ = itsQ[state][i] ;
			}
			return maxQ ;
		}
} ;

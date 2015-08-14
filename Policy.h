#pragma once
#include <vector>
#include <random>
#include <stdlib.h>
#include <chrono>

enum LearningType {QLEARNING, SARSA, SARSALAMBDA} ;

class Policy
{
	public:
		Policy(): alpha(0.9), gamma(0.9), eps(0.01), lambda(0.9), learning(QLEARNING) {}
		~Policy() {}
		
		// Initialise state-action value table to zeros
		void SetQ(int numStates, int numActions){
			vector< double > temp(numActions,0.0) ;
			for (int i = 0; i < numStates; i++){
				itsQ.push_back(temp) ;
				itsTrace.push_back(temp) ;
			}
		}
		
		// Initialise state-action trace to zeros
		void SetTrace(){
			itsTrace.clear() ;
			int numStates = itsQ.size() ;
			int numActions = itsQ[0].size() ;
			vector< double > temp(numActions,0.0) ;
			for (int i = 0; i < numStates; i++){
				itsTrace.push_back(temp) ;
			}
		}
		
		void SetLearningType(LearningType algorithm) {learning = algorithm ;}
		
		// Select the next action based on the assigned learning algorithm
		int NextAction(int state0, int action, int reward, int state1){
			itsReward = reward ;
			
			// Compute the next best action
			int tempMaxQ = itsQ[state1][0] ;
			int tempAction = 0 ;
			for (unsigned i = 0; i < itsQ[state1].size(); i++){
				if (itsQ[state1][i] > tempMaxQ){
					tempMaxQ = itsQ[state1][i] ;
					tempAction = i ;
				}
			}
			
			// Epsilon greedy exploration
			int newAction ;
			int numActions = itsQ[0].size() ;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() ;
			default_random_engine generator(seed);
			uniform_real_distribution<double> distribution(0.0,1.0);
			double pAct = distribution(generator) ;
			if (pAct <= eps)
				newAction = rand() % numActions ;
			else
				newAction = tempAction ;
			
			// Calculate temporal difference
			double deltaQLearn = itsReward + gamma*GetMaxQ(state1) - itsQ[state0][action] ;
			double deltaSARSA = itsReward + gamma*itsQ[state1][newAction] - itsQ[state0][action] ;
			
			// Learning backup
			switch (learning){
				case QLEARNING:
					// Q-learning
					itsQ[state0][action] = itsQ[state0][action] + alpha*deltaQLearn ;
					break ;
				case SARSA:
					// SARSA
					itsQ[state0][action] = itsQ[state0][action] + alpha*deltaSARSA ;
					break ;
				case SARSALAMBDA:
					// SARSA(lambda)
					for (unsigned i = 0; i < itsTrace.size(); i++){
						for (unsigned j = 0; j < itsTrace[i].size(); j++){
							if (state0 == (int)i && action == (int)j)
								itsTrace[i][j] = 1 ;
							else
								itsTrace[i][j] *= gamma*lambda ;
							itsQ[i][j] = itsQ[i][j] + alpha*deltaSARSA*itsTrace[i][j] ;
						}
					}
					break ;
			}
			
			return newAction ;
		}
	private:
		vector< vector<double> > itsQ ;
		vector< vector<double> > itsTrace ;
		double alpha ;
		double gamma ;
		double eps ;
		double lambda ;
		LearningType learning ;
		double itsReward ;
		
		// Return the maximum Q-value for the given state
		double GetMaxQ(int state){
			double maxQ = itsQ[state][0] ;
			for (unsigned i = 0; i < itsQ[state].size(); i++){
				if (itsQ[state][i] > maxQ)
					maxQ = itsQ[state][i] ;
			}
			return maxQ ;
		}
} ;

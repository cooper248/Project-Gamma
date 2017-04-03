//
//  main.cpp
//  Project Gamma
//
//  Created by Cooper Richardson on 4/2/17.
//  Copyright © 2017 Cooper Richardson. All rights reserved.

#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>



#define crRand (double)rand()/RAND_MAX
#define PI 3.1415296

using namespace std;



// Global Constants
int numCities;
int numMutations;

class city{
public:
    double cityLocX;
    double cityLocY;
    void initCity();
};

class policy{
public:
    vector<city> cityOrder;
    double fitness;
    double evalFitness(vector<city>);
    void initPolicy(vector<city>);
};

void initCities(vector<city>* allCities){
    cout<<"How many cities would you like to create?: ";
    cin>>numCities;
    cout<<endl;
    
    //cout<<"How many mutation cycles would you like to complete?: ";
    //cin>>numMutations;
    //cout<<endl;
    
    for(int i=0; i<numCities; i++){
        city A;
        A.initCity();
        allCities->push_back(A);
    }
};

void city::initCity(){
    srand(time(0));
    cityLocX = rand()% 100 + crRand - crRand;
    cityLocY = rand()% 100 + crRand - crRand;
};

void policy::initPolicy(vector<city> policyChange){
    cityOrder = policyChange;
    fitness = evalFitness(cityOrder);
};

double policy::evalFitness(vector<city> cityOrder){
    double fitness;
    double distance = 0.0;
    
    for(int i = 0; i<numCities-1; i++)
    {
        double xDistance = abs(cityOrder.at(i).cityLocX - cityOrder.at(i+1).cityLocX );
        double yDistance = abs(cityOrder.at(i).cityLocY - cityOrder.at(i+1).cityLocY );
        distance = distance + sqrt((xDistance*xDistance)+(yDistance*yDistance));
    }
    
    fitness = distance;
    return fitness;
};

void initPolicies(vector<policy>* population, vector<city>* allCities){
    
    int numPolicies = 100;
    vector<city> policyChange = *allCities;
    srand(time(NULL));
    population->clear();
    
    for (int i = 0; i < numPolicies; i++)
    {
        for(int i=0; i<numCities; i++)
        {
            vector<city> hold;
            city B;
            B.initCity();
            hold.push_back(B);
            
            int pullLocation1 = rand()% numCities;
            int pullLocation2 = rand()% numCities;
                
            while(pullLocation1 == 0 || pullLocation2 == 0 || pullLocation1 == pullLocation2)
            {
                pullLocation1 = rand()% numCities;
                pullLocation2 = rand()% numCities;
            }
                
            hold[0]=policyChange.at(pullLocation1);
                
            policyChange.at(pullLocation1) = policyChange.at(pullLocation2);
            policyChange.at(pullLocation2) = hold[0];
        }
        policy A;
        A.initPolicy(policyChange);
        population->push_back(A);
    }
    assert(population->size() == numPolicies);
};

int main() {
    
    vector<policy> population;
    vector<city> allCities;
    
    // Initialize cities
    initCities(&allCities);

    // Initialize policies
    initPolicies(&population, &allCities);
    
    for(int i = 0; i < population.size(); i++ )
    {
        cout<<"Fitness: "<<population.at(i).fitness<<endl;
        //cout<<"Fitness: "<<population.at(i).fitness
        
    }

    for(int i = 0; i<numCities; i++)
    {
        cout<<"X location: "<<allCities.at(i).cityLocX<<endl;
    }
    
    // Start EA (for)
    
        // Replicate -> Mutate
    
        // Evaluate fitnesses
    
        // Downselect -> Binary tournament
    
    
}









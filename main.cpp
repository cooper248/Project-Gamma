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
using namespace std;

// Global Constants
int numCities;
int numMutations;
int populationSize = 100;

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
    vector<city> mutatePolicy(vector<city>);
};

void initCities(vector<city>* allCities){
    cout<<"How many cities would you like to create?: ";
    cin>>numCities;
    cout<<endl;
    
    cout<<"How many mutation cycles would you like to complete?: ";
    cin>>numMutations;
    cout<<endl;
    
    for(int i=0; i<numCities; i++){
        city A;
        A.initCity();
        allCities->push_back(A);
    }
};

void city::initCity(){
    
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
    vector<city> policyChange = *allCities;
    srand(time(NULL));
    population->clear();
    
    for (int i = 0; i < populationSize; i++)
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
    assert(population->size() == populationSize);
};

vector<city> policy::mutatePolicy(vector<city> mutatePolicy){
    
    vector<city> mutate = mutatePolicy;
    int numChanges = numCities * .1;
    if(numCities<10){
        numChanges = 1;
    }
    
    for(int i = 0; i<numChanges; i++)
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
        
        hold[0]=mutate.at(pullLocation1);
        
        mutate.at(pullLocation1) = mutate.at(pullLocation2);
        mutate.at(pullLocation2) = hold[0];
        hold.clear();
    }
    assert(mutate.size()==numCities);
    return mutate;
};

vector<policy> replicatePop(vector<policy>* population){
    
    // take in current population
    vector<policy> mutatePop = *population;
    assert(mutatePop.size() == populationSize);
    
    // replicate and mutate
    for(int i =0; i<populationSize; i++)
    {
        policy B;
        vector<city> mutate = B.mutatePolicy(population->at(i).cityOrder);
        B.initPolicy(mutate);
        mutatePop.push_back(B);
    }
    
    assert(mutatePop.size() == populationSize * 2);
    return mutatePop;
};

vector<policy> downSelect(vector<policy>* mutatedPopulation){
    vector<policy> population;
    population.clear();
    assert(population.size() == 0);
    
    while(population.size()<populationSize)
    {
        int pullLocation1 = rand()% populationSize*2;
        int pullLocation2 = rand()% populationSize*2;
        
        while(pullLocation1 == pullLocation2)
        {
            pullLocation1 = rand()% populationSize*2;
        }
        
        if(mutatedPopulation->at(pullLocation1).fitness < mutatedPopulation->at(pullLocation2).fitness){
            population.push_back(mutatedPopulation->at(pullLocation1));
        }
        
        else if (mutatedPopulation->at(pullLocation1).fitness >= mutatedPopulation->at(pullLocation2).fitness)
        {
            population.push_back(mutatedPopulation->at(pullLocation2));
        }
    }
    assert(population.size()== populationSize);
    return population;
};


int main() {
    srand(time(NULL));
    
    vector<policy> population;
    vector<city> allCities;
    vector<policy> mutatedPopulation;
    
    // Initialize cities
    initCities(&allCities);

    // Initialize policies
    initPolicies(&population, &allCities);
    
    
    double average = 0.0;
    double min = 10000;
    double max = 0;
    int minRepeat = 0;
    cout<<"Original Fitnesses: "<<endl;
    for(int i=0; i<populationSize; i++){
        cout<<"Policy Fitness: "<<population.at(i).fitness<<endl;
        average = average + population.at(i).fitness;
        if(population.at(i).fitness<min){
            min = population.at(i).fitness;
        }
        if(population.at(i).fitness>max){
           max = population.at(i).fitness;
        }
        if(population.at(i).fitness==min){
            minRepeat++;
        }
    }
    cout<<"AVERAGE: "<<average/100<<" MIN: "<<min<<" MAX: "<<max<<" Min repeat: "<<minRepeat<<endl;
    
    
    // Start EA (for)
    for(int i = 0; i< numMutations; i++)
    {
        // Replicate -> Mutate
        mutatedPopulation = replicatePop(&population);
    
        // Downselect -> Binary tournament
        population.clear();
        population = downSelect(&mutatedPopulation);
    }
    
    average = 0;
    min = 10000;
    max=0;
    minRepeat = 0;
    cout<<endl<<" - - - - - - - - - - - - - - - - - - - - - - - - "<<endl<<"Final Fitnesses: "<<endl;
    for(int i=0; i<populationSize; i++){
        cout<<"Policy Fitness: "<<population.at(i).fitness<<endl;
        average = average + population.at(i).fitness;
        if(population.at(i).fitness<min){
            min = population.at(i).fitness;
        }
        if(population.at(i).fitness>max){
            max = population.at(i).fitness;
        }
        if(population.at(i).fitness==min){
            minRepeat++;
        }
    }
    cout<<" AVERAGE: "<<average/100<<endl<<" MIN: "<<min<<endl<<" MAX: "<<max<<endl<<" Min repeat: "<<minRepeat<<endl;
    
}









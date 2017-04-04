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
    
    // Assert that distance was calculated                                                              // Both LR 7 & LR 8
    assert(distance != 0);
    
    
    fitness = distance;
    
    // MR 2 - fitness is based on distance traveled                                                        // MR 2
    assert(fitness == distance);
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
        
        // LR 6 - Doesnt visit same city twice                                                                      // LR 6
        for(int i =0; i<policyChange.size(); i++)
        {
            for(int j = 0; j<policyChange.size(); j++)
            {
                if(i != j){
                    assert(policyChange.at(i).cityLocX != policyChange.at(j).cityLocX);
                }
            }
        }
        
        policy A;
        A.initPolicy(policyChange);
        
        // MR 3 all policies have a fitness                                                                         // MR 3
        assert(A.fitness > 10);
        population->push_back(A);
    }
    

    
    // LR 5 - Start in same city                                                                                       // LR 5
    for(int i = 0; i<populationSize-1; i++)
    {
        assert(population->at(i).cityOrder.at(0).cityLocX == population->at(i+1).cityOrder.at(0).cityLocX);
    }
    
    // MR 1 - population is filled with policies and the size is greater than 1 (equals the desired size)              // MR 1
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
    
    //Test that slight mutation occured                                                                           // LR 4
    int repeat=0;
    for(int i= 0; i<numCities; i++){
        if(mutate.at(i).cityLocX == mutatePolicy.at(i).cityLocX){
            repeat++;
        }
    
    }
    
    assert(repeat<numCities);
    assert(mutate.size()==numCities);
    return mutate;
};

vector<policy> replicatePop(vector<policy>* population){
    
    // take in current population
    vector<policy> mutatePop = *population;
    assert(mutatePop.size() == populationSize);
    int pullLocation;
    
    // replicate and mutate
    for(int i =0; i<populationSize; i++)
    {
        policy B;
        pullLocation = rand()% mutatePop.size();
        vector<city> mutate = B.mutatePolicy(mutatePop.at(pullLocation).cityOrder);
        B.initPolicy(mutate);
        mutatePop.push_back(B);
    }
    
    // MR 5 - Can return to carrying capacity (Mutated population doubles back to 200)                         // MR 5
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
    
    // MR 4 - Downselect cuts parent population in half - Downselection                                           // MR 4
    assert(population.size() == mutatedPopulation->size()/2);
    return population;
};

void printInfo(vector<policy> population, vector<double>* averages, vector<double>* minimum, vector<double>* maximum){
    
    double average = 0.0;
    double min = 10000;
    double max = 0;
    int minRepeat = 0;
    for(int i=0; i<populationSize; i++){
        //cout<<"Policy Fitness: "<<population.at(i).fitness<<endl;
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
    average = average/100;
    cout<<"AVERAGE: "<<average<<" MIN: "<<min<<" MAX: "<<max<<" Min repeat: "<<minRepeat<<endl;
    averages->push_back(average);
    minimum->push_back(min);
    maximum->push_back(max);
};

void writeToFile(vector<double>* averages, vector<double>* minimum, vector<double>* maximum){

    ofstream allValueEA;
    allValueEA.open("EvoAlgValues");
    for(int i =0; i<numMutations; i++)
    {
    allValueEA << averages->at(i) << "\t" << minimum->at(i) << "\t" << maximum->at(i) << "\n";
    }
    allValueEA.close();
};


int main() {
    srand(time(NULL));
    
    vector<policy> population;
    vector<city> allCities;
    vector<policy> mutatedPopulation;
    vector<double> averages;
    vector<double> maximum;
    vector<double> minimum;
    
    // Initialize cities
    initCities(&allCities);

    // Initialize policies
    initPolicies(&population, &allCities);
    
    cout<<"Original Fitnesses: "<<endl;
    printInfo(population, &averages, &minimum, &maximum);
    
    // Start EA (for)
    for(int i = 0; i< numMutations; i++)
    {
        // Replicate -> Mutate
        mutatedPopulation = replicatePop(&population);
    
        // Downselect -> Binary tournament
        population.clear();
        population = downSelect(&mutatedPopulation);
        cout<<endl;
        printInfo(population, &averages, &minimum, &maximum);
    }
    cout<<endl<<" - - - - - - - - - - - - - - - - - - - - - - - - "<<endl<<"Final Fitnesses: "<<endl;
    printInfo(population, &averages, &minimum, &maximum);
    writeToFile( &averages, &minimum, &maximum );
    
    
    //cout<<"THAT GOOD GOOD: "<<averages.at(0)/averages.at(averages.size()-1)<<endl;
    
    // HR 1 - Assert that final average is at least 15 percent smaller than original                              // HR 1
    assert(averages.at(0)/averages.at(averages.size()-1) > 1.15);
    
    
    //HR 2 3 4 will be determined during write up by user input of 10 25 and 100 cities
}



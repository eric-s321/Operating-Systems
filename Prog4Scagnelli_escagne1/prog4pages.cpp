#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <fstream>
#include "prog4pages.hpp"

#define PAGE_ACCESSES 10000
#define UNIQUE_LOOPING_PAGES 50

struct clockEntry{
    int page;
    int usedBit;
};

//Max is inclusive
int randomInt(int min, int max){
    return rand() % (max + 1 - min);
}

bool dequeContains(deque<int> fifo, int page){
    if(find(fifo.begin(), fifo.end(), page) != fifo.end()) //page is in the deque
        return true;
    return false;
}

bool vectorContains(vector<int> mem, int page){
    if(find(mem.begin(), mem.end(), page) != mem.end())
        return true;
    return false;
}

float optimal(int *workLoad, unsigned int memSize){
    int hits = 0;
    int misses = 0;

    vector<int> mem;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        if(mem.size() < memSize){ //mem not yet full
            if(!vectorContains(mem, workLoad[i])){
                mem.push_back(workLoad[i]);
                misses++;
            }
            else{
                hits++;
            }
        }
        else{ //mem is full, use optimal page replacement
            if(vectorContains(mem, workLoad[i])){
                hits++;
            }
            else{
                int evictIndex = getOptimalEvictPage(mem, workLoad, i);                  
                mem[evictIndex] = workLoad[i];
                misses++;
            }
        }
    }

    return (hits / (float) (hits + misses)) * 100;
}

int getOptimalEvictPage(vector<int> mem, int *workLoad, int workLoadIndex){

    int distances[mem.size()];
    for(unsigned int i = 0; i < mem.size(); i++){
        int page = mem[i];
        int distanceAway = 0;
        int index = workLoadIndex;
        for(;index < PAGE_ACCESSES; index++){
            if(workLoad[index] == page)
               break; 
            distanceAway++;
        }    
        distances[i] = distanceAway;
    }

    int evictIndex = distance(distances, max_element(distances, distances + mem.size()));
    return evictIndex;
}

float LRU(int *workLoad, unsigned int memSize){
    int hits = 0;
    int misses = 0;
    deque<int> mem;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        if(mem.size() < memSize){ //mem not yet full
            if(!dequeContains(mem, workLoad[i])){
                mem.push_back(workLoad[i]);
                misses++;
            }
            else{
                mem.erase(find(mem.begin(),mem.end(),workLoad[i])); //Take current page in workload off deque
                mem.push_back(workLoad[i]); //Puts the page we just accessed at the back of the deque
                hits++;
            }
        }
        else{ //mem is full
            if(dequeContains(mem, workLoad[i])){
                mem.erase(find(mem.begin(),mem.end(),workLoad[i])); //Take current page in workload off deque
                mem.push_back(workLoad[i]); //Puts the page we just accessed at the back of the deque
                hits++;
            }
            else{
                mem.pop_front(); //Take LRU page off deque
                mem.push_back(workLoad[i]); 
                misses++;
            }
        }
    }
    return (hits / (float) (hits + misses)) * 100;
}

//returns the index if the page is in memory
//returns -1 if not in memory
int inMemory(vector<int> mem, int page){
    for(unsigned int i = 0; i < mem.size(); i++){
        if(mem[i] == page)
            return i;
    }
    return -1;
}

float clock(int *workLoad, unsigned int memSize){
    int hits = 0;
    int misses = 0;
    vector<int> mem;
    clockEntry pages [100];
    unsigned int index = 0;

    //Initialize all unique pages used bits to 0
    for(int i = 0; i < 100; i++){
        pages[i].page = i;
        pages[i].usedBit = 0;
    }

    for(int i = 0; i < PAGE_ACCESSES; i++){
        if(mem.size() < memSize){ //mem not yet full
            if(!vectorContains(mem,workLoad[i])){ //page is not in mem
                mem.push_back(workLoad[i]);
                misses++;
            }
            else{
                hits++;
            }
        }
        else{ //mem is full
            int nextPage = workLoad[i];
            if(vectorContains(mem,nextPage)){ //page is already in mem
                hits++;            
            }
            else{ //Clock alg to find replacement
                bool pageReplaced = false;
                while(!pageReplaced){
                    int memIndex = inMemory(mem, pages[index].page);
                    if(pages[index].usedBit == 0 && memIndex != -1){ //page has not been recently used and is in memory
                        mem[memIndex] = nextPage;
                        pageReplaced = true;
                    }
                    else if(pages[index].usedBit == 1)//page has been recently used
                        pages[index].usedBit = 0;
                    
                    if(index + 1 == 100)
                        index = 0;
                    else
                        index++;
                }
                misses++;
            }
        }
    }
    return (hits / (float) (hits + misses)) * 100;
}



float fifo(int *workLoad, unsigned int memSize){
    int hits = 0;
    int misses = 0;
    deque<int> fifo;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        if(fifo.size() < memSize){ //fifo not yet full
            if(!dequeContains(fifo, workLoad[i])){
                fifo.push_back(workLoad[i]);
                misses++;
            }
            else{
                hits++;
            }
        }
        else{ //fifo is full
            int nextPage = workLoad[i];
            if(dequeContains(fifo, nextPage)){
                hits++;            
            }
            else{
                fifo.pop_front();
                fifo.push_back(nextPage);
                misses++;
            }
        }
    }
    return (hits / (float) (hits + misses)) * 100;
}

float random(int *workLoad, unsigned int memSize){
    int hits = 0;
    int misses = 0;

    vector<int> mem;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        if(mem.size() < memSize){ //mem not yet full
            if(!vectorContains(mem, workLoad[i])){
                mem.push_back(workLoad[i]);
                misses++;
            }
            else{
                hits++;
            }
        }
        else{ //fifo is full
            int nextPage = workLoad[i];
            if(vectorContains(mem, nextPage)){
                hits++;            
            }
            else{
                int randIndex = randomInt(0,memSize-1);
                mem[randIndex] = nextPage;
                misses++;
            }
        }
    }
    return (hits / (float) (hits + misses)) * 100;
}


int* getNoLocalityWorkload(){
    int *workLoad = new int[PAGE_ACCESSES];

    for(int i = 0; i < PAGE_ACCESSES; i++){
        workLoad[i] = randomInt(0,99);     
    } 

    return workLoad;
}

int* get80_20Workload(){
    int *workLoad = new int[PAGE_ACCESSES];

    for(int i = 0; i < PAGE_ACCESSES; i++){
        int value = randomInt(0,99);
        if(value > 0 && value < 79){ //Will occur 80% of the time
            workLoad[i] = randomInt(0,19);  //First 20 pages are the hot pages
        }
        else //Will occur 20% of the time
            workLoad[i] = randomInt(20,99); //Last 80 are the cold pages
    }

    return workLoad;
}

int* getLoopingWorkload(){
    int *workLoad = new int[PAGE_ACCESSES];

    for(int i = 0; i < PAGE_ACCESSES; i++){
        workLoad[i] = i % UNIQUE_LOOPING_PAGES; 
    }

    return workLoad;
}

void noLocalitySimulation(){
    int *noLocalityWorkload = getNoLocalityWorkload();    

    vector<float> optResults;
    for(int i = 5; i <= 100; i += 5)
        optResults.push_back(optimal(noLocalityWorkload, i));

    vector<float> fifoResults;
    for(int i = 5; i <= 100; i += 5)
        fifoResults.push_back(fifo(noLocalityWorkload, i));

    vector<float> randomResults;
    for(int i = 5; i <= 100; i += 5)
        randomResults.push_back(random(noLocalityWorkload, i));
    
    vector<float> LRUResults;
    for(int i = 5; i <= 100; i += 5)
        LRUResults.push_back(LRU(noLocalityWorkload, i));

    vector<float> clockResults;
    for(int i = 5; i <= 100; i += 5)
        clockResults.push_back(clock(noLocalityWorkload, i));

    ofstream outFile;
    outFile.open("no-locality.csv");

    if(!outFile.is_open()){
        perror("Creating file failed.");
        exit(EXIT_FAILURE);
    }

    outFile << "#cache,OPT,LRU,FIFO,RAND,CLOCK\n";
    
    int memSize = 5;
    for(unsigned int i = 0; i < optResults.size(); i++){
        outFile << memSize << "," << optResults[i] << "," << 
            LRUResults[i] << "," << fifoResults[i] << "," <<
            randomResults[i] << "," << clockResults[i] << "\n";
        memSize += 5;
    }

    outFile.close();

    delete []noLocalityWorkload;
}

void eighty20Simulation(){
    int *eighty20Workload = get80_20Workload();

    vector<float> optResults;
    for(int i = 5; i <= 100; i += 5)
        optResults.push_back(optimal(eighty20Workload, i));

    vector<float> fifoResults;
    for(int i = 5; i <= 100; i += 5)
        fifoResults.push_back(fifo(eighty20Workload, i));

    vector<float> randomResults;
    for(int i = 5; i <= 100; i += 5)
        randomResults.push_back(random(eighty20Workload, i));
    
    vector<float> LRUResults;
    for(int i = 5; i <= 100; i += 5)
        LRUResults.push_back(LRU(eighty20Workload, i));

    vector<float> clockResults;
    for(int i = 5; i <= 100; i += 5)
        clockResults.push_back(clock(eighty20Workload, i));
    
    ofstream outFile;
    outFile.open("80-20.csv");

    if(!outFile.is_open()){
        perror("Creating file failed.");
        exit(EXIT_FAILURE);
    }

    outFile << "#cache,OPT,LRU,FIFO,RAND,CLOCK\n";

    int memSize = 5;
    for(unsigned int i = 0; i < optResults.size(); i++){
        outFile << memSize << "," << optResults[i] << "," << 
            LRUResults[i] << "," << fifoResults[i] << "," <<
            randomResults[i] << "," << clockResults[i] << "\n";
        memSize += 5;
    }

    outFile.close();

    delete []eighty20Workload;
}

void loopingSimulation(){
    int *loopingWorkload = getLoopingWorkload();

    vector<float> optResults;
    for(int i = 5; i <= 100; i += 5)
        optResults.push_back(optimal(loopingWorkload, i));

    vector<float> fifoResults;
    for(int i = 5; i <= 100; i += 5)
        fifoResults.push_back(fifo(loopingWorkload, i));

    vector<float> randomResults;
    for(int i = 5; i <= 100; i += 5)
        randomResults.push_back(random(loopingWorkload, i));
    
    vector<float> LRUResults;
    for(int i = 5; i <= 100; i += 5)
        LRUResults.push_back(LRU(loopingWorkload, i));
    
    vector<float> clockResults;
    for(int i = 5; i <= 100; i += 5)
        clockResults.push_back(clock(loopingWorkload, i));
    
    ofstream outFile;
    outFile.open("looping.csv");

    if(!outFile.is_open()){
        perror("Creating file failed.");
        exit(EXIT_FAILURE);
    }

    outFile << "#cache,OPT,LRU,FIFO,RAND,CLOCK\n";

    int memSize = 5;
    for(unsigned int i = 0; i < optResults.size(); i++){
        outFile << memSize << "," << optResults[i] << "," << 
            LRUResults[i] << "," << fifoResults[i] << "," <<
            randomResults[i] << "," << clockResults[i] << "\n";
        memSize += 5;
    }

    outFile.close();

    delete []loopingWorkload;
}

int main(){

    srand((unsigned)time(0));
    
    noLocalitySimulation();
    eighty20Simulation();
    loopingSimulation();

    return 0;
}

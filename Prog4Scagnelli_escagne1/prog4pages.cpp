#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <deque>
#include <vector>
#include "prog4pages.hpp"

#define PAGE_ACCESSES 10000
#define UNIQUE_LOOPING_PAGES 50

using namespace std;

//Max is inclusive
int randomInt(int min, int max){
    return rand() % (max + 1 - min);
}

bool fifoContains(deque<int> fifo, int page){
    if(find(fifo.begin(), fifo.end(), page) != fifo.end()) //page is in the deque
        return true;
    return false;
}

bool vectorContains(vector<int> mem, int page){
    if(find(mem.begin(), mem.end(), page) != mem.end())
        return true;
    return false;
}

float fifo(int *workLoad, unsigned int memSize){
    int hits = 0;
    int misses = 0;
    deque<int> fifo;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        if(fifo.size() < memSize){ //fifo not yet full
            if(!fifoContains(fifo, workLoad[i])){
                fifo.push_back(workLoad[i]);
                misses++;
//                cout << "MISS" << endl;
            }
            else{
                hits++;
//                cout << "HIT" << endl;
            }
        }
        else{ //fifo is full
            int nextPage = workLoad[i];
            if(fifoContains(fifo, nextPage)){
                hits++;            
//                cout << "HIT" << endl;
            }
            else{
                fifo.pop_front();
                fifo.push_back(nextPage);
                misses++;
//                cout << "MISS" << endl;
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
//                cout << "MISS" << endl;
            }
            else{
                hits++;
//                cout << "HIT" << endl;
            }
        }
        else{ //fifo is full
            int nextPage = workLoad[i];
            if(vectorContains(mem, nextPage)){
                hits++;            
//                cout << "HIT" << endl;
            }
            else{
                int randIndex = randomInt(0,memSize-1);
                mem[randIndex] = nextPage;
                misses++;
//                cout << "MISS" << endl;
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

int main(){

    srand((unsigned)time(0));
    
    int *noLocalityWorkload = getNoLocalityWorkload();    
    int *eighty20Workload = get80_20Workload();
    int *loopingWorkload = getLoopingWorkload();

/*
    cout << "No Locality Workload..." << endl;
    for(int i = 0; i < PAGE_ACCESSES; i++){
        cout << noLocalityWorkload[i] << endl;
    }

    cout << "\n\n 80/20 Workload..." << endl;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        cout << eighty20Workload[i] << endl;
    }

    cout << "\n\n Looping Workload..." << endl;

    for(int i = 0; i < PAGE_ACCESSES; i++){
        cout << loopingWorkload[i] << endl;
    }
*/

//    int testWorkload [] = {0,1,2,0,1,3,0,3,1,2,1};
    cout << "Fifo results..." << endl;
    for(int i = 5; i <= 100; i += 5){
        float hitRate = fifo(noLocalityWorkload, i);
        cout << "Hit rate is " << hitRate << endl;
    }

    cout << "Random results..." << endl;
    for(int i = 5; i <= 100; i += 5){
        float hitRate = random(noLocalityWorkload, i);
        cout << "Hit rate is " << hitRate << endl;
    }

    delete []noLocalityWorkload; 
    delete []eighty20Workload;
    delete []loopingWorkload;

    return 0;
}

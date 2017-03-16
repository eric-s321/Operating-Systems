#include <cstdlib>
#include <iostream>
#include <ctime>
#include "prog4.h"

#define PAGE_ACCESSES 100
#define UNIQUE_LOOPING_PAGES 50

using namespace std;

//Max is inclusive
int randomInt(int min, int max){
    return rand() % (max + 1 - min);
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

    delete []noLocalityWorkload; 
    delete []eighty20Workload;
    delete []loopingWorkload;

    return 0;
}

#include <deque>
#include <vector>


using namespace std;

int randomInt(int min, int max);
bool dequeContains(deque<int> fifo, int page);
bool vectorContains(vector<int> mem, int page);
float optimal(int *workLoad, unsigned int memSize);
int getOptimalEvictPage(vector<int> mem, int *workLoad, int workLoadIndex);
float LRU(int *workLoad, unsigned int memSize);
float fifo(int *workLoad, unsigned int memSize);
float random(int *workLoad, unsigned int memSize);
int *getNoLocalityWorkload();
int* get80_20Workload();
int* getLoopingWorkload();
void noLocalitySimulation();
void eighty20Simulation();
void loopingSimulation();

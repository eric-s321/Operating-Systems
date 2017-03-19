#include <deque>
#include <vector>


using namespace std;

int randomInt(int min, int max);
bool fifoContains(deque<int> fifo, int page);
bool vectorContains(vector<int> mem, int page);
float optimal(int *workLoad, unsigned int memSize);
int getOptimalEvictPage(vector<int> mem, int *workLoad, int workLoadIndex);
float fifo(int *workLoad, unsigned int memSize);
float random(int *workLoad, unsigned int memSize);
int *getNoLocalityWorkload();
int* get80_20Workload();
int* getLoopingWorkload();

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define MAX_HEAP_SIZE 1000

// how many bytes of the heap are free?
int bytesLeft = MAX_HEAP_SIZE;

// need lock and condition too
pthread_cond_t c;
pthread_mutex_t m;

void *allocate(int size) {
    pthread_mutex_lock(&m);
    while (bytesLeft < size)
        pthread_cond_wait(&c, &m);
    void *ptr = ...; // get mem from heap
    bytesLeft -= size;
    pthread_mutex_unlock(&m);
    return ptr;
}

void *free(int size) {
    pthread_mutex_lock(&m);
    bytesLeft += size;
    pthread_cond_signal(&c); // whom to signal??
    pthread_mutex_unlock(&m);
}

int main(){

    pthread_t a1;
    pthread_t a2;
    pthread_t f1;

    int mem1 = 100;
    int mem2 = 10;
    int mem3 = 50;

    pthread_create(&a1, NULL, allocate, &mem1);
    pthread_create(&a2, NULL, allocate, &mem2);
    pthread_create(&f1, NULL, free, &mem3);

    pthread_join(a1, NULL);

    return 0;
}

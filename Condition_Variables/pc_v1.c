#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

int buffer;
int count = 0; // initially, empty

void put(int value) {
    assert(count == 0);
    count = 1;
    buffer = value;
}

int get() {
    assert(count == 1);
    count = 0;
    return buffer;
}

void *producer(void *arg) {
    int i;
    int loops = *(int *) arg;
    for (i = 0; i < loops; i++) {
        put(i);
    }
    return NULL;
}

void *consumer(void *arg) {
//    int i;
    while (1) {
        int tmp = get();
        printf("%d\n", tmp);
    }
    return NULL;
}

int main(){

    pthread_t p;
    pthread_t c;
    int loops = 10;
    pthread_create(&p, NULL, producer, &loops);
    pthread_create(&c, NULL, consumer, NULL);
    //pthread_exit(NULL);
    pthread_join(p, NULL);

    return 0;
}

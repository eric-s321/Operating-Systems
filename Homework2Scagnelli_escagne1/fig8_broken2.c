#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

int buffer;
int count = 0; // initially, empty

pthread_cond_t cond;
pthread_mutex_t mutex;

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
        pthread_mutex_lock(&mutex); // p1
        while (count == 1) // p2
            pthread_cond_wait(&cond, &mutex); // p3
        put(i); // p4
        pthread_cond_signal(&cond); // p5
        pthread_mutex_unlock(&mutex); // p6
    }
    return NULL;
}

void *consumer(void *arg) {
    int i;
    int loops = *(int *) arg;
    for (i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex); // c1
        while (count == 0) // c2
            pthread_cond_wait(&cond, &mutex); // c3
        int tmp = get(); // c4
        pthread_cond_signal(&cond); // c5
        pthread_mutex_unlock(&mutex); // c6
        printf("%d\n", tmp);
    }
    return NULL;
}

int main(){

    pthread_t p;
    pthread_t c1;
    pthread_t c2;
    int loops = 10;
    pthread_create(&p, NULL, producer, &loops);
    pthread_create(&c1, NULL, consumer, &loops);
    pthread_create(&c2, NULL, consumer, &loops);
    //pthread_exit(NULL);
    pthread_join(p, NULL);

    return 0;
}

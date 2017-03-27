#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit() {
    done = 1;
    pthread_cond_signal(&c);
}

void thr_join() {
    if(done == 0)
        pthread_cond_wait(&c, &m);
}

void *child(void *arg) {
    printf("child\n");
    thr_exit();
    return NULL;
}


int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    pthread_t p;
    pthread_create(&p, NULL, child, NULL);
    thr_join();
    printf("parent: end\n");
    return 0;
}

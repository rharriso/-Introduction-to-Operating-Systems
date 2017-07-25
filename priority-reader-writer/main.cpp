#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t read_phase = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_phase = PTHREAD_COND_INITIALIZER;

void* readFunc(void *pArg) {
    int* p = (int*) pArg;
    int myNum = *p;
    printf("Read Thread number %d\n", myNum);
   return 0;
}

void* writeFunc(void *pArg) {
    int* p = (int*) pArg;
    int myNum = *p;
    printf("Write Thread number %d\n", myNum);
    return 0;
}

int main() {

    pthread_t readers[NUM_THREADS]; // create set of pthreads
    pthread_t writers[NUM_THREADS]; // create set of pthreads
    int readerVals[NUM_THREADS];
    int writerVals[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        readerVals[i] = i;
        pthread_create(&readers[i], NULL, readFunc, &readerVals[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        writerVals[i] = i + 100;
        pthread_create(&writers[i], NULL, readFunc, &writerVals[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    return 0;
}
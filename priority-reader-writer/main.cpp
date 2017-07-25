#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 100

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t read_phase = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_phase = PTHREAD_COND_INITIALIZER;
int resourceCounter = 0;
int readerCount = 0;

int dataToWrite = 0;

void* readFunc(void *pArg) {
    pthread_mutex_lock(&counter_mutex);
        while(resourceCounter == -1) {
            pthread_cond_wait(&read_phase, &counter_mutex);
        }
        resourceCounter++;
        readerCount++;
    pthread_mutex_unlock(&counter_mutex);

    printf("Reading data %d\n", dataToWrite);

    pthread_mutex_lock(&counter_mutex);
        resourceCounter--;
        readerCount--;
        if(resourceCounter == 0) {
            pthread_cond_signal(&write_phase);
        }
    pthread_mutex_unlock(&counter_mutex);
   return 0;
}

void* writeFunc(void *pArg) {
    pthread_mutex_lock(&counter_mutex);
    while(resourceCounter != 0) {
        pthread_cond_wait(&write_phase, &counter_mutex);
    }
    resourceCounter = -1;
    pthread_mutex_unlock(&counter_mutex);

    dataToWrite++;
    printf("Writing data %d; Reader Count: %d\n", dataToWrite, readerCount);

    pthread_mutex_lock(&counter_mutex);
        resourceCounter = 0;
        pthread_cond_broadcast(&read_phase);
        pthread_cond_signal(&write_phase);
    pthread_mutex_unlock(&counter_mutex);

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
        pthread_create(&writers[i], NULL, writeFunc, &writerVals[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    return 0;
}
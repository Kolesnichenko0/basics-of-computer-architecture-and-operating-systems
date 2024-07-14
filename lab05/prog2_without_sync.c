#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#define BUFFER_SIZE 256
char buffer[BUFFER_SIZE];

#define NUM_THREADS 2

int var = 0;
bool end_flag = false;

void* thread_func(void* arg) {
    int id = *(int*)arg; //thread identifier
    printf("thread %d is running\n",id);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    while(!end_flag){
        var++;
        printf("Thread %d increased var to %d\n", id, var);
        sleep(1);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    //Creating threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, (void*)&thread_ids[i]);
        sleep(1);
    }
    sleep(1);
    printf("Enter + to increase value of var(Enter 'end' to finish):\n");
    while(end_flag == false && fgets(buffer, BUFFER_SIZE, stdin)) { //Reading the string from the console
        if (strncmp(buffer, "end", 3) == 0) {
            end_flag = true;
            for (int i = 0; i < NUM_THREADS; i++) {
                thread_ids[i] = i;
                pthread_cancel(threads[i]);
            }
        }
    }

    printf("Final value of var: %d\n", var);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define LEFT_FORK ((i-1) + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT_FORK ((i-1) + 1) % NUM_PHILOSOPHERS

pthread_mutex_t forks[NUM_PHILOSOPHERS];

void *philosopher(void *arg) {
    int i = (*(int*)arg)+1;

        // looking at the table
        printf("Philosopher %d is looking at the table\n", i);

    while (1) {
        // try to take both forks
        if (pthread_mutex_trylock(&forks[LEFT_FORK]) == 0) {
            printf("Philosopher %d took left fork %d\n", i, LEFT_FORK+1);
            if (pthread_mutex_trylock(&forks[RIGHT_FORK]) == 0) {
                printf("Philosopher %d took right fork %d\n", i, RIGHT_FORK+1);

                printf("Philosopher %d is eating spaghetti\n", i);
                sleep(1); // eating time

                pthread_mutex_unlock(&forks[RIGHT_FORK]);
                printf("Philosopher %d put back right fork %d\n", i, RIGHT_FORK+1);

                pthread_mutex_unlock(&forks[LEFT_FORK]);
                printf("Philosopher %d put back left fork %d\n", i, LEFT_FORK+1);

                break; // finished eating
            } else {
                pthread_mutex_unlock(&forks[LEFT_FORK]);
                printf("Philosopher %d put back left fork %d\n", i, LEFT_FORK+1);
            }
        }
        sleep(1);
    }

    pthread_exit(NULL);

}

int main() {
    printf("Press any key to start the program...\n");
    getchar();
    pthread_t threads[NUM_PHILOSOPHERS];
    int args[NUM_PHILOSOPHERS];

    // initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        args[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &args[i]);
    }

    // join threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Threads joined\n");
    // destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }


    return 0;
}
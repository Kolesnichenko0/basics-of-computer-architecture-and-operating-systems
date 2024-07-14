#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];
pthread_mutex_t mutex; //The mutex object is used to synchronize threads
pthread_cond_t write_cond, read_cond; //The condition variables for synchronization
bool end_flag = false; //Flag indicating whether data entry has ended
bool write_flag = false; //Flag indicating whether data has been written to the buffer
bool read_flag = false; //Flag indicating whether data has been read from the buffer

void* write_thread(void* arg) {
    printf("write_thread is running\n");

    while (!end_flag) {
        pthread_mutex_lock(&mutex); //Grabbing the mutex

        printf("Enter message to write to buffer (Enter 'end' to finish):\n");
        fgets(buffer, BUFFER_SIZE, stdin); //Reading the entered string from the console

        if (strncmp(buffer, "end", 3) == 0) {
            end_flag = true;
            pthread_mutex_unlock(&mutex); //Freeing the mutex
            pthread_exit(NULL);

        } else {
            write_flag = true;
            read_flag = false;
        }

        pthread_cond_signal(&read_cond); //Signaling the read thread that data has been written

        pthread_cond_wait(&write_cond, &mutex); //Waiting for the read thread to signal that data has been read

        pthread_mutex_unlock(&mutex); //Freeing the mutex

        sleep(2);
    }

    pthread_exit(NULL); //Finishing the thread
}

void* read_thread(void* arg) {
    printf("read_thread is running\n");

    while (!end_flag) {
        pthread_mutex_lock(&mutex); //Grabbing the mutex

        while (!write_flag && !end_flag) {
            pthread_cond_wait(&read_cond, &mutex); //Waiting for the write thread to signal that data has been written
        }

        if (!end_flag) {
            printf("Buffer contents: %s", buffer);
            buffer[0] = '\0';

            read_flag = true;
            write_flag = false;
            pthread_cond_signal(&write_cond); //Signaling the write thread that data has been read
        }

        pthread_mutex_unlock(&mutex); //Freeing the mutex

        sleep(3);

    }

    pthread_exit(NULL); //Finishing the thread
}

int main() {
    pthread_t write_t, read_t;
    //Initialising the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }
    //Initialising the condition variables
    if (pthread_cond_init(&write_cond, NULL) != 0 || pthread_cond_init(&read_cond, NULL) != 0) {
        perror("Condition variable initialization failed");
        exit(EXIT_FAILURE);
    }
    //Creating a recording thread
    if (pthread_create(&write_t, NULL, write_thread, NULL) != 0) {
        perror("Write thread creation failed");
        exit(EXIT_FAILURE);
    }
    sleep(3);
    //Creating a reading thread
    if (pthread_create(&read_t, NULL, read_thread, NULL) != 0) {
        perror("Read thread creation failed");
        exit(EXIT_FAILURE);
    }

    //Waiting for the streams to finish their work
    if (pthread_join(write_t, NULL)) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(read_t, NULL)) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Threads joined\n");
    //Destroying mutex
    pthread_mutex_destroy(&mutex);

    return 0;

}
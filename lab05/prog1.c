#define _REENTRANT //Enabling the multithreading option
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define FILENAME "test.txt"
#define BUFFER_SIZE 256
char buffer[BUFFER_SIZE];
pthread_mutex_t mutex; //The mutex object is used to synchronize threads
bool end_flag = false; //Flag indicating whether data entry has ended

void* write_thread(void* arg) {
    printf("write_thread is running\n");
    FILE* fp;

    while (!end_flag) {

        pthread_mutex_lock(&mutex); //Grabbing the mutex
        printf("Enter message to write to file (Enter 'end' to finish):\n");
        fgets(buffer, BUFFER_SIZE, stdin); //Reading the entered string from the console
        if (strncmp(buffer, "end", 3) == 0) {
            end_flag = true;
        }

        fp = fopen(FILENAME, "a+"); //Opening a file in post mode
        if (fp != NULL && !end_flag) {
            fprintf(fp, "%s", buffer);
            fclose(fp);
        }
        pthread_mutex_unlock(&mutex); //Freeing the mutex
        sleep(2);

    }

    pthread_exit(NULL); //Finishing the thread
}

void* read_thread(void* arg) {
    printf("read_thread is running\n");
    FILE* fp;

    while (!end_flag) {
        pthread_mutex_lock(&mutex); //Grabbing the mutex
        if(!end_flag) {
            printf("test.txt:\n");
            fp = fopen(FILENAME, "r"); //opening a file in read-only mode ("r").
            if (fp != NULL) {
                while (fgets(buffer, BUFFER_SIZE, fp)) { //Reading the string from the file
                    printf("%s", buffer);
                    buffer[0] = '\0';
                }
                fclose(fp);
            }

            pthread_mutex_unlock(&mutex); //Freeing the mutex

            sleep(3);
        }

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
    if(pthread_join(write_t, NULL)) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(read_t, NULL)) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }


    printf("Threads joined\n");
    //Destroying mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
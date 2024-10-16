/**
 * GM
 * Date: 10/13/24
 * Operating Systems
 * Assignment 3 - Question 2
 * 
 * This program simulates a coffee shop where baristas need access 
 * to all three coffee machines to make coffee.
 * 
 * We use mutex locks to ensure that no two baristas can use the same 
 * machine at the same time.
 * 
 * Compile: gcc Q2.c -o q2 -lpthread
 * Run: ./q2
 */

#include <stdio.h>     // For input/output
#include <unistd.h>    // For sleep function - simulate time making coffee
#include <pthread.h>   // For thread creation and mutex

// Declarations

// Mutex Locks for each Coffee Machine
pthread_mutex_t left_machine;
pthread_mutex_t center_machine;
pthread_mutex_t right_machine;

// Function declaration
void* barista_make_coffee(void* id);

int main() {
    // Initialize the mutex locks using NULL for default attributes
    pthread_mutex_init(&left_machine, NULL);   // Initialize left machine mutex
    pthread_mutex_init(&center_machine, NULL); // Initialize center machine mutex
    pthread_mutex_init(&right_machine, NULL);  // Initialize right machine mutex

    // Creating thread variables for 5 baristas
    pthread_t barista1, barista2, barista3, barista4, barista5;
    
    // ids for each barista
    int id1 = 1, id2 = 2, id3 = 3, id4 = 4, id5 = 5;

    // Create each thread for the baristas
    pthread_create(&barista1, NULL, barista_make_coffee, &id1);
    pthread_create(&barista2, NULL, barista_make_coffee, &id2);
    pthread_create(&barista3, NULL, barista_make_coffee, &id3);
    pthread_create(&barista4, NULL, barista_make_coffee, &id4);
    pthread_create(&barista5, NULL, barista_make_coffee, &id5);

    // Wait for each thread to finish
    pthread_join(barista1, NULL);
    pthread_join(barista2, NULL);
    pthread_join(barista3, NULL);
    pthread_join(barista4, NULL);
    pthread_join(barista5, NULL);

    // Destroying the Mutexes after all threads are done
    pthread_mutex_destroy(&left_machine);
    pthread_mutex_destroy(&center_machine);
    pthread_mutex_destroy(&right_machine);

    return 0;
}

// Barista function that simulates the process of making coffee
void* barista_make_coffee(void* arg) {
    // Get thread id from the argument we put in the thread
    int id = *(int*)arg;

     // Barista tries to make coffee
    printf("Barista %d is starting to work.\n", id);
    sleep(1);

    // Attempt to lock all three coffee machines
    printf("Barista %d is waiting for machines...\n", id);
    sleep(1);

    // lock machines
    pthread_mutex_lock(&left_machine);
    printf("\n");
    printf("Barista %d acquired the left machine\n", id);
    pthread_mutex_lock(&center_machine);
    printf("Barista %d acquired the center machine\n", id);
    pthread_mutex_lock(&right_machine);
    printf("Barista %d acquired the right machine\n", id);

    // All machines acquired, start making coffee
    printf("Barista %d has all machines and is making coffee...\n", id);
    sleep(3); // Simulate time taken to make coffee
    
    // Coffee done
    printf("Barista %d has finished making coffee.\n", id);
    sleep(1);

    // Unlock machines after making coffee
    // unlocking from right to left to prevent deadlock
    pthread_mutex_unlock(&right_machine);
    printf("Barista %d released the right machine.\n", id);
    pthread_mutex_unlock(&center_machine);
    printf("Barista %d released the center machine.\n", id);
    pthread_mutex_unlock(&left_machine);
    printf("Barista %d released the left machine.\n", id);
    printf("Barista %d has finished the work cycle.\n", id);

    return NULL;
}

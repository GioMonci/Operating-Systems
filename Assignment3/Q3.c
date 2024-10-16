/**
 * GM
 * Date: 10/13/24
 * Operating Systems
 * Assignment 3 - Question 3
 * 
 * This program simulates a parking lot where cars try and find parking spots. 
 * There are 5 parking spaces, and cars will wait for spots if all spots are taken.
 * 
 * We use semaphores to track available parking spots and a mutex to sync the printf
 * to the thread thats outputing info, so that the terminal output looks cleaner.
 * 
 * Note: The mutex is used to stop multiple cars from printing at the same time,
 * so their output doesn’t overlap and to safely update and display the number of 
 * available parking spots without messing up the shared data.
 * 
 * Compile: gcc Q3.c -o q3 -lpthread -lrt
 * Run: ./q3
 */

#include <stdio.h>     // For input/output
#include <stdlib.h>    // For rand() and srand()
#include <unistd.h>    // For sleep() function
#include <pthread.h>   // For thread creation and mutex
#include <semaphore.h> // For semaphore creation

// Definitions
#define NUM_SPOTS 5 // Number of parking spots
#define NUM_CARS 10 // Number of cars trying to park

// Global declarations
sem_t parking_spots;               // Semaphore to rep the available parking spots
pthread_mutex_t mutex;            //  Mutex to sync printf with thread
int available_spots = NUM_SPOTS; //   Variable to track available parking spots (used for printf)

// Function declaration for the parking lot simulation
void* parking_lot_sim(void* arg);

int main() {
    // Seed for random number generator to simulate random arrival times
    srand(time(NULL));

    // Array to hold thread identifiers for each car
    pthread_t cars[NUM_CARS];  
    // Array to store car IDs (for easy reference in threads)
    int car_ids[NUM_CARS];    

    // Initialize the semaphore to the number of parking spots
    sem_init(&parking_spots, 0, NUM_SPOTS);

    // Initialize the mutex for sync printf access
    pthread_mutex_init(&mutex, NULL);

    // Create threads for each car in the parking lot simulation
    for (int i = 0; i < NUM_CARS; i++) {
        car_ids[i] = i + 1;  // Assign car IDs
        pthread_create(&cars[i], NULL, parking_lot_sim, &car_ids[i]); // Create thread
    }

    // Wait for each car thread to finish
    for (int i = 0; i < NUM_CARS; i++) {
        pthread_join(cars[i], NULL);
    }

    // Clean up by destroying the semaphore and mutex
    sem_destroy(&parking_spots);  
    pthread_mutex_destroy(&mutex);

    // Ending statment
    printf("End of Day, all cars parked and left.\n");

    return 0;
}

// Function to simulate the parking lot process for each car
void* parking_lot_sim(void* arg) {
    // Get car ID passed as an argument to the thread
    int car_id = *(int*)arg; // used for printf

    // Simulate the car arriving at a random time (1-3 seconds)
    sleep(rand() % 3 + 1);

    // Car arrives and tries to find a parking spot
    printf("Car %d arrives and is trying to find a parking spot...\n", car_id);

    // Wait for an available parking spot
    sem_wait(&parking_spots);  // If no spots are available, the car waits here

    // Lock the mutex to sync to a thread
    pthread_mutex_lock(&mutex);
    available_spots--;  // Decrement the number of available spots (for printf)
    printf("Car %d parks. Available spots: %d\n", car_id, available_spots);
    pthread_mutex_unlock(&mutex);  // Unlock the mutex after print and updating avalib_spots

    // Simulate the car being parked for a random duration (1-5 seconds)
    sleep(rand() % 5 + 1);

    // When the car leaves, free up a spot
    pthread_mutex_lock(&mutex); // Lock the mutex to sync to a thread
    available_spots++;  // Increment the number of available spots
    printf("Car %d leaves. Available spots: %d\n", car_id, available_spots);
    pthread_mutex_unlock(&mutex); // Unlock the mutex after print and updating avalib_spots

    // Signal that a spot has been freed by incrementing the semaphore
    sem_post(&parking_spots);

    return NULL;
}

/**
 * GM
 * Date: 9/18/24
 * Operating Systems
 * Assignment 2 - Question 2
 * 
 * This program calculates the range, standard deviation,
 * and interquartile range of numbers passed in the command line
 * 
 * It uses three worker threads to perform each calculation,
 * parent thread outputs values once workers exit.
 * 
 * gcc Question2.c -o question2 -lm
 * 
 * ./question2 90 81 78 95 79 72 85
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// global variables
int range = 0;
double stddev = 0.0;
double iqr = 0.0;

// funct declarations
void* calcRange(void* args);
void* calcStdDev(void* args);
void* calcIqr(void* args);

int compare(const void *a, const void *b);
double median(int arr[], int start, int end);

typedef struct{
    // need this struct to hold the array and its size
    // for the threads because they hold one arg.
    int *arr; 
    int size;
} ArrayArgs;

int main(int argc, char *argv[]) {

    // error if not enough arguments are inputted
    if (argc < 2) {
        printf("Not enough arguments\n");
        return 1;
    }

    // creating an array to store values from argv
    // need to covert them from string to ints

    int arraySize = argc - 1; // Exclude program name
    // dynamic array size
    int argvArray[arraySize]; 

    // convert argv strings to ints -> store in argvArray
    for (int i = 0; i < arraySize; i++) {
        argvArray[i] = atoi(argv[i + 1]);
    }

    // creating single argument for the threads
    // that has the array and its size
    ArrayArgs args = { argvArray, arraySize };

    // making thread ids
    pthread_t rangeThread, stddevThread, iqrThread;

    // creating threads
    // NULL for attr to use default attributes
    pthread_create(&rangeThread, NULL, calcRange, (void*)&args);
    pthread_create(&stddevThread, NULL, calcStdDev, (void*)&args);
    pthread_create(&iqrThread, NULL, calcIqr, (void*)&args);

    // wait for threads to complete and join
    pthread_join(rangeThread, NULL);
    pthread_join(stddevThread, NULL);
    pthread_join(iqrThread, NULL);

    // print report
    printf("The range value is: %d\n", range);
    printf("The standard Deviation value is: %.2f\n", stddev);
    printf("The interquartile range value is: %.2f\n", iqr);

    return 0;
}

// function to calc range
void* calcRange(void* args) {

    // getting array and size
    ArrayArgs* arrayArgs = (ArrayArgs*)args;
    int* arr = arrayArgs->arr;
    int size = arrayArgs->size;

    // settings max and min to first value in arr
    int min, max;
    min = max = arr[0];

    // go through array to find min and max
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    // get range by subtracting max - min
    range = max - min;
    pthread_exit(0);
}

// function to calc the std dev
void* calcStdDev(void* args) {
    
    // getting array and size
    ArrayArgs* arrayArgs = (ArrayArgs*)args;
    int* arr = arrayArgs->arr;
    int size = arrayArgs->size;

    // if size of arr is less than 2
    if (size < 2) {
        stddev = -1; // Not enough values to calc std dev
        pthread_exit(0);
    }

    double sum = 0.0, mean, variance = 0.0;

    // calc the sum of numbers in array
    for (int i = 0; i < size; i++) {
        sum = sum + arr[i];
    }

    // calc mean by dividing the sum of array by its size
    mean = sum / size;

    // calc variance (top half of std dev) ∑ni (xi−(sample mean))2
    for (int i = 0; i < size; i++) {
        variance = variance + pow(arr[i] - mean, 2);
    }

    // divide variance by (n - 1) to get sample variance not population
    // variance / n-1
    variance = variance / (size - 1);

    // return std dev -> square root (variance)
    stddev = sqrt(variance);
    pthread_exit(0);
}

// function to compare two ints
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

double median(int arr[], int start, int end){
    // start is the beggining of the half of the array we are on
    // end is the end of the half of the array we are on
    // Example: lower half of array (Q1)-> start = 0; because index 0
    // end is (midpoint -1) because the end of the lowerhalf is 1 less than midpoint
    int size = end - start + 1; // numb of values in the range

    if(size % 2 == 0){
        // if the size is even then median is average of the two middle numbs
        return (arr[start + size /2-1] + arr[start+size / 2]) / 2.0;
    } else{
        // if the size is odd the the middle would be the median
        return arr[start + size /2];
    }
}

// function to calc iqr
void* calcIqr(void* args) {

    // getting array and size
    ArrayArgs* arrayArgs = (ArrayArgs*)args;
    int* arr = arrayArgs->arr;
    int size = arrayArgs->size;

    // if size of arr less than 4 then we can't do iqr
    if (size < 4) {
        iqr = -1; // not enough numbs to calculate iqr
        pthread_exit(0);
    }

    // sort array using qsort - put in numerical order
    qsort(arr, size, sizeof(int), compare);
    
    // variables for storing Q1 and Q3
    double q1, q3;
    // middle of array
    int midpoint = size/2;

    // getting median of q1
    q1 = median(arr, 0, midpoint -1);

   // getting median of q3
    if(size % 2 == 0){
        // if the size of the array is even
        q3 = median(arr, midpoint, size -1);
    } else {
        // odd size
        q3 = median(arr, midpoint + 1, size -1);
    }

    // iqr is q3 - q1
    
    iqr = q3 - q1;
    pthread_exit(0);
}

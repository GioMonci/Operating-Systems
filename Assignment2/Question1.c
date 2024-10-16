/**
 * GM
 * Date: 9/18/24
 * Operating Systems
 * Assignment 2 - Question 1
 * 
 * This program generates collatz sequence using child process
 * created by fork().
 * 
 * The child does the sequence and the parent waits
 * for the child to finish before exiting.
 * 
 * gcc Question1.c -o question1
 * 
 * ./question1 35
 * ./question1 8
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// funct declarations
void collatzSequence(int num);

int main(int argc, char *argv[]){

    // error if no number provided
    if(argc < 2){
        printf("Provide int as a Command Line ARG\n");
        return 1; // return error
    }

    // Convert argv character string to int value
    int argvToInt = atoi(argv[1]);

    // error if zero is input
    if(argvToInt == 0){
        printf("Invalid input: 0 inputed\n");
        return 1;
    }

    // error if negative int is input
    if(argvToInt <= -1){
        printf("Invalid Input: negative int %d was inputted\n", argvToInt);
        return 1;
    }

    // create child process
    pid_t pid = fork();

    if(pid < 0){
        // Fork didn't work, no child process created
        perror("fork failed");
        return 1; // return error
    } else if(pid == 0){
        // Child process
            // This block is executed only in the child process
        printf("Child process PID: %d is running Collatz sequence for %d:\n", getpid(), argvToInt);
        collatzSequence(argvToInt);
        exit(0); // Terminate child process after completing task
        printf("Child complete\n");

    } else {
        // Parent process
        printf("Parent process PID: %d \n", getpid());
        wait(NULL); // Wait for child process to finish
        printf("Child complete\n");

    }

    return 0;
}

void collatzSequence(int num){

    // Print the starting number
    printf("%d", num);

    // Generate Collatz sequence
    while (num != 1) {
        // if even num
        if (num % 2 == 0) {
            num = num / 2;
        } else {
            // if odd num
            num = 3 * num + 1;
        }
        printf(", %d", num);
    }
    printf("\n");
}

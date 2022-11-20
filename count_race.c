#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long *array;

long array_length = 10000000; // Global variable that the sequential function cont1s() will return its answer into
int count = 0; // Global variable modifiied by multiple threads
int correct_count = 0; // Number of threads

int ThreadsNumber = 16; // Number of threads


int count1s()
// Requires: Array containing integers
// Effects: Returns the number of 1s in it.
// Modifies: correct_count
// Testing Strategy: This function is tested by the main function by passing an array of 1s and 0s and checking if the function returns the correct number of 1s
{

    for (int i = 0; i < array_length; i++)
    {
        if (array[i] == 1)
        {

            correct_count++;
        }
    }
    return correct_count;
}

void *threadCounting(void *id)
// Requires: The id of the thread. The id ranges from 0 to n-1 where n=number of threads.
// Effects: Returns the number of ones the threads have counted.
// Modifies: The global variable count
// Testing Strategy: Array sizes of 100,1000,10000,1000000,10000000,100000000,1000000000 with threads of sizes 1,2,4,8,16,32,64.
{
    long ID = (long)id;

    long numberofIterations = array_length / ThreadsNumber;

    long startIndex = ID * numberofIterations;

    long endIndex = startIndex + numberofIterations;

    // If this is the last thread, let it iterate till the end of the array
    if (array_length - endIndex < numberofIterations)
    {
        endIndex = array_length;
    }

    for (startIndex; startIndex < endIndex; startIndex++)
    {
        if (array[startIndex] == 1)
            count++;
    }
}

int main()
{
    clock_t start_time, check_time1, end_time;

    array = (long *)malloc(array_length * sizeof(long));

    for (long i = 0; i < array_length; i++)
    {
        array[i] = rand() % 5 + 1;
    }
    // Start time for sequential function
    start_time = clock();
    correct_count = count1s();
    // End time for sequential function and also start time of parallel programming
    check_time1 = clock();

    pthread_t T[ThreadsNumber];

    for (int i = 0; i < ThreadsNumber; i++)
    {
        pthread_create(&T[i], NULL, &threadCounting, (void *)i);
    }

    for (int i = 0; i < ThreadsNumber; i++)
    {
        pthread_join(T[i], NULL);
    }

    end_time = clock();
    double total_Time_sequential = ((double)(check_time1 - start_time)) / CLOCKS_PER_SEC;
    double total_Time_parallel = ((double)(end_time - check_time1)) / CLOCKS_PER_SEC;

    printf("The number of Threads: \t\t\t%d\n", ThreadsNumber);
    printf("The Time Taken sequentially: \t\t%f\n", total_Time_sequential);
    printf("The Correct count of 1's= \t\t%d\n", correct_count);
    printf("The Time Taken - parallel programming: \t%f\n", total_Time_parallel);
    printf("The Count of 1s with multiple threads: \t%d\n", count);
    free(array);

    return 0;
}
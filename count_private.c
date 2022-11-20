// There is no performance improvement because the threads are not synchronized.
// Private variables are not shared across threads, so each thread has its own copy. This means that each thread can update its own copy without having to worry about race conditions. However, because the threads are not synchronized, they may not all be updating the same copy of the variable. This can lead to inconsistency in the results.


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long *array; // Global array
long array_length = 100; // Global variable that the sequential function cont1s() will return its answer into
int correct_count = 0; // Global variable modifiied by multiple threads
int ThreadsNumber = 8; // Number of threads
long *threads_independantResults; // Array to store the results of each thread

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

// this function will be executed by each thread
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

    // This is a private variable for each thread, to which each thread is modifying to find the number of 1s in their range
    int private_counter = 0;

    // If this is the last thread, let it iterate till the end of the array

    if (array_length - endIndex < numberofIterations)
    {
        endIndex = array_length;
    }

    for (startIndex; startIndex < endIndex; startIndex++)
    {
        if (array[startIndex] == 1)
            private_counter++;
    }

    // Store the number of 1s found by a thread in an array such that if thread is of id 2, we store at index 2.
    threads_independantResults[ID] = private_counter;
}

int main()
{

    printf("Array Length\tThreads Number\tSequential Time\tCorrect Count\tParallel Time\tCount\n");

    for (int x = 0; x < 100; x++) // testing 100 times
    {
        array_length = 10;
        ThreadsNumber = 1;

        for (int i = 0; i < 7; i++)
        {
            array_length *= 10;
            ThreadsNumber = 1;
            for (int j = 0; j < 6; j++)
            {
                correct_count = 0;
                int count = 0;

                ThreadsNumber *= 2;

                clock_t start_time, check_time1, end_time;

                array = (long *)malloc(array_length * sizeof(long));
                threads_independantResults = (long *)malloc(ThreadsNumber * sizeof(long));

                for (long i = 0; i < array_length; i++)
                {
                    array[i] = rand() % 5 + 1;
                }

                start_time = clock();
                correct_count = count1s();
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
                double total_Time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

                for (int j = 0; j < ThreadsNumber; j++)
                {
                    count += threads_independantResults[j];
                }

                double total_Time_sequential = ((double)(check_time1 - start_time)) / CLOCKS_PER_SEC;
                double total_Time_parallel = ((double)(end_time - check_time1)) / CLOCKS_PER_SEC;

                printf("%ld\t\t%d\t\t\t%f\t%d\t\t%f\t%d\n", array_length, ThreadsNumber, total_Time_sequential, correct_count, total_Time_parallel, count);

                free(array);
                free(threads_independantResults);
                return 0;
            }
        }
    }
}
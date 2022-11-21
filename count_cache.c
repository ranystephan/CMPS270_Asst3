#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct fill_cache // Structure to pass to the threads
{
    long id;
    char *characters;
};

long *array; // Global array
long array_length = 10000000; // Global variable that the sequential function cont1s() will return its answer into
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

void *threadCounting(void *my_struct)
// Requires: my_struct is a pointer to a struct fill_cache
// Modifies: my_struct->characters
// Effects:  Fills my_struct->characters with random characters
// Testing Strategy: This function is tested by the main function by 

{
    struct fill_cache *struct_ptr = (struct fill_cache *)my_struct; // Cast the void pointer to a struct fill_cache pointer
    long numberofIterations = array_length / ThreadsNumber;
    long startIndex = (struct_ptr->id) * numberofIterations;
    long endIndex = startIndex + numberofIterations;

    int private_counter = 0;
    int remaining_bytes = 256000 - ((numberofIterations * 8) + (3 * sizeof(long)) + sizeof(int));
    struct_ptr->characters = (char *)malloc(remaining_bytes * sizeof(char));

    if (array_length - endIndex < numberofIterations) // If this is the last thread, let it iterate till the end of the array
    {
        endIndex = array_length;
    }

    for (startIndex; startIndex < endIndex; startIndex++) // Iterate through the array and count the number of 1s
    {
        if (array[startIndex] == 1)
            private_counter++;
    }

    threads_independantResults[struct_ptr->id] = private_counter; // Store the number of 1s found by a thread in an array such that if thread is of id 2, we store at index 2.
    free(struct_ptr->characters); // Free the memory allocated to the characters array
}

int main()
{
    int count = 0;

    clock_t start_time, end_time;
    start_time = clock();

    array = (long *)malloc(array_length * sizeof(long));
    threads_independantResults = (long *)malloc(ThreadsNumber * sizeof(long));

    for (long i = 0; i < array_length; i++)
    {
        array[i] = rand() % 5 + 1;
    }

    correct_count = count1s();

    pthread_t T[ThreadsNumber];

    for (int i = 0; i < ThreadsNumber; i++)
    {

        struct fill_cache fill;
        fill.id = i;
        pthread_create(&T[i], NULL, &threadCounting, &fill);
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

    printf("# of Threads: %d\n", ThreadsNumber);
    printf("Time Taken: %f\n", total_Time);
    printf("Correct count of 1's= %d\n", correct_count);
    printf("Count of 1s with multiple threads: %d\n", count);
    free(array);
    free(threads_independantResults);
    return 0;
}
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>

using namespace std;

void equalSplit(unsigned int N, unsigned int M, unsigned int* elements, unsigned int* threadSize) {
    // Evaluate the number of elements per thread
    unsigned int elementsPerThread = N / M;
    unsigned int remainingElements = N % M;

    // Adjust the threadSize array for the remaining elements, if any
    for (unsigned int i = 0; i < M; i++) {
        threadSize[i] = elementsPerThread + (i < remainingElements ? 1 : 0);
    }
}

void shuffleAndSplit(unsigned int N, unsigned int M, unsigned int* elements, unsigned int* threadSize) {
    // Shuffle the elements
    random_device rd;
    mt19937 g(rd()); // Mersenne Twister engine for good randomness
    shuffle(elements, elements + N, g); // Shuffle the elements array

    // Equal split the shuffled elements
    equalSplit(N, M, elements, threadSize);
}

void cardDistribution(unsigned int N, unsigned int M, unsigned int* elements, unsigned int* threadSize) {
    // Call equalSplit the evaluate the number of elements per thread
    equalSplit(N, M, elements, threadSize);

    // Fill the elements array with zeros
    fill(elements, elements + N, 0);

    // Distribute the numbers across elements array in the desired order
    unsigned int currentThreadIndex = 0;
    unsigned int currentThreadOffsets[M] = {0};

    // Fill the currentThreadOffsets with the start index based on the threadSize
    for (unsigned int i = 1; i < M; i++) {
        currentThreadOffsets[i] = currentThreadOffsets[i - 1] + threadSize[i - 1];
    }

    for (unsigned int i = 0; i < N; i++) {
        // Assign the current element to the current thread
        elements[currentThreadOffsets[currentThreadIndex]] = i + 1;

        // Move to the next thread
        currentThreadOffsets[currentThreadIndex]++;
        currentThreadIndex = (currentThreadIndex + 1) % M;
    }
}

void splitAndShuffle(unsigned int N, unsigned int M, unsigned int* elements, unsigned int* threadSize) {

    // Divide the orderedElements array into parts of size M and shuffle each part
    unsigned int elementsPerThread = N / M;
    unsigned int remainingElements = N % M;

    // Adjust the threadSize array for the remaining elements, if any
    for (unsigned int i = 0; i < M; i++) {
        threadSize[i] = elementsPerThread + (i < remainingElements ? 1 : 0);
    }

    // Generate a new array with the ordered elements
    unsigned int orderedElements[N] = {0};
    for (unsigned int i = 0; i < N; i++) {
        orderedElements[i] = i + 1;
    }
    // Shuffle each block of size M
    random_device rd;
    mt19937 g(rd()); // Mersenne Twister engine for good randomness

    for (unsigned int i = 0; i < elementsPerThread; i++) {
        shuffle(orderedElements + i * M, orderedElements + (i + 1) * M, g);
    }

    // Shuffle the remaining elements
    shuffle(orderedElements + elementsPerThread * M, orderedElements + elementsPerThread * M + remainingElements, g);

    unsigned int currentThreadIndex = 0;
    unsigned int currentThreadOffsets[M] = {0};

    // Fill the currentThreadOffsets with the start index based on the threadSize
    for (unsigned int i = 1; i < M; i++) {
        currentThreadOffsets[i] = currentThreadOffsets[i - 1] + threadSize[i - 1];
    }

    // Assign the shuffled elements to the elements array
    // Each threadSize will contain one element of each block of size M
    // The remaining elements will be assigned to the first threads
    for (unsigned int i = 0; i < M; i++) {
        for (unsigned int j = 0; j < threadSize[i]; j++) {
            elements[currentThreadOffsets[i] + j] = orderedElements[i + j * M];
        }
    }
}

void workloadBalance(unsigned int N, unsigned int M, unsigned int* elements, unsigned int* threadSize) {

    // Calculate the workload for each element and the total workload using squares
    unsigned int workload[N] = {0};
    unsigned int totalWorkload = 0;

    for (unsigned int i = 0; i < sqrt(N); i++) {
        // Select the range for the current square workload
        unsigned int start = i * i;
        unsigned int end = (i + 1) * (i + 1) - 1;
        end = min(end, N); // Ensure we don't go beyond N

        // Evaluate the workload for each element with this workload
        for (unsigned int j = start; j <= end; j++) {
            totalWorkload += i;
            workload[j] = i;
        }
    }

    // Calculate the target workload per thread
    unsigned int targetWorkload = totalWorkload / M;

    // Calculate the thread size
    unsigned int currentThread = 0;
    unsigned int currentWorkload = 0;

    for (unsigned int i = 0; i < N; i++) {
        currentWorkload += workload[i];
        threadSize[currentThread]++;

        if (currentWorkload > targetWorkload && currentThread < M - 1) {
            currentThread++;
            currentWorkload = 0;
        }
    }
}

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>
#include <thread>

using namespace std;

void printSets(unsigned int N, unsigned int M, unsigned int elements[], unsigned int threadSize[], bool printElements=true) {
    
    unsigned int previousThreads = 0;

    for (unsigned int i = 0; i < M; i++) {
        // Print the elements of eachs set
        cout << "Set " << i + 1 << " (size " << threadSize[i] << "): ";
        for (unsigned int j = 0; j < threadSize[i]; j++) {
            cout << elements[previousThreads + j] << " ";
        }
        cout << endl;

        previousThreads += threadSize[i];
    }
}


void executeApproach(unsigned int N, unsigned int M, unsigned int elements[], unsigned int threadSize[], int idxThread, bool isPrime[], char aproach) {
    // Calculate the start and end index for the current thread
    int start_idx = 0;
    for (int i = 0; i < idxThread; i++) {
        start_idx += threadSize[i];
    }

    // Calculate the end index
    int end_idx = start_idx + threadSize[idxThread];

    // Evaluate the elements in the current thread
    for (int i = start_idx; i < end_idx; i++) {
        int operations = 0;
        if (aproach == 'i') {
            isPrime[i] = is_prime_it(elements[i], operations);
        } 
        else if (aproach == 'r') {
            isPrime[i] = is_prime_re(elements[i], operations);
        }
    }
}


void giveThreadsWork(unsigned int N, unsigned int M, unsigned int elements[], unsigned int threadSize[], bool isPrime[], char aproach='i') {
    // Create the threads
    thread threads[M];

    // Give work to each thread
    for (unsigned int i = 0; i < M; i++) {
        threads[i] = thread(executeApproach, N, M, elements, threadSize, i, ref(isPrime), aproach);
    }

    // Join the threads
    for (unsigned int i = 0; i < M; i++) {
        threads[i].join();
    }
}
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>

#include "src/prime.hpp"
#include "src/setDistribution.hpp"
#include "src/utils.hpp"

using namespace std;

int main() {

    unsigned int N = 107; // Range to evaluate (1 to N)
    unsigned int M = 10; // Number of threads

    // Create the elements and threadSize arrays
    unsigned int elements[N] = {0}; // Array to store the numbers to evaluate (from 1 to N, by default)
    unsigned int threadSize[N] = {0}; // Array to store the thread size (number of elements to evaluate in each thread)
    bool isPrime[N] = {false}; // Array to store the prime numbers

    // Fill the elements array
    for (unsigned int i = 0; i < N; i++) {
        elements[i] = i + 1;
    }

    int approach = 1;

    for (int approach = 1; approach <= 5; approach++) {

        cout << "Approach " << approach << endl;

        switch (approach) {
            case 1: // Split in equal parts, without balance
                equalSplit(N, M, elements, threadSize);
                break;

            case 2: // Shuffle and split in equal parts
                shuffleAndSplit(N, M, elements, threadSize);
                break;

            case 3: // "Card distribution" approach (round-robin with remaining elements)
                cardDistribution(N, M, elements, threadSize);
                break;

            case 4: // Split and shuffle
                splitAndShuffle(N, M, elements, threadSize);
                break;

            case 5: // Workload estimation
                workloadBalance(N, M, elements, threadSize);
                break;
            
            default:
                break;
        }
        
        // Print the sets
        printSets(N, M, elements, threadSize);

        cout << "\n\n" << endl;

        // Restart the elements and threadSize arrays
        for (unsigned int i = 0; i < N; i++) {
            elements[i] = i + 1;
        }

        for (unsigned int i = 0; i < M; i++) {
            threadSize[i] = 0;
        }
    }

    return 0;
}

// The line to compile and exec
// g++ -std=c++11 main.cpp -o main; ./main 
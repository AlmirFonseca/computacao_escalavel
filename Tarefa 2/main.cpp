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

#define OVERSAMPLING_FACTOR 3

int iNumPrime = 0;

int main() {

    unsigned int M = 7; // Number of threads

    char methods[2] = {'r', 'i'}; // Methods to evaluate (r for recursive, i for iterative)

    // Create the CSV file
    createCSV(M);

    // Iterate over the methods (recursive and iterative)
    for (char method : methods) {

        cout << "Method " << method << endl;

        // Iterate over the range to evaluate (10^i, i = 1, 2, ...)
        for (int k = 1; k < 3; k++) {
            
            unsigned int N = 10^k; // Range to evaluate (1 to N)

            // Create the elements and threadSize arrays
            unsigned int elements[N] = {0}; // Array to store the numbers to evaluate (from 1 to N, by default)
            unsigned int threadSize[N] = {0}; // Array to store the thread size (number of elements to evaluate in each thread)
            bool isPrime[N] = {false}; // Array to store the prime numbers

            // Fill the elements array
            for (unsigned int i = 0; i < N; i++) {
                elements[i] = i + 1;
            }

            // Iterate over the indexing approaches
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
                // printSets(N, M, elements, threadSize, false);

                // cout << "\n\n" << endl;

                // Iterate over the oversampling factor
                for (unsigned int i = 1; i <= OVERSAMPLING_FACTOR; i++) {

                    cout << "Oversampling factor " << i << endl;

                    // Give work to each thread
                    giveThreadsWork(N, M, elements, threadSize, isPrime, method);

                    // Print the prime numbers
                    for (unsigned int i = 0; i < N; i++) {
                        if (isPrime[i]) {
                            cout << i + 1 << " is prime" << endl;
                            iNumPrime++;
                        }
                    }

                    cout << "\nThere are " << iNumPrime << " prime numbers" << endl;

                    appendCSV(N, M, elements, threadSize, isPrime, 0, method, 0);

                    // Restart the elements and threadSize arrays
                    for (unsigned int i = 0; i < N; i++) {
                        elements[i] = i + 1;
                    }

                    for (unsigned int i = 0; i < M; i++) {
                        threadSize[i] = 0;
                    }
                }
            }
        }
    }

    return 0;
}

// The line to compile and exec
// g++ -std=c++11 main.cpp -o main; ./main 


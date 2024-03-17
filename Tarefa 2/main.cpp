#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>
#include <fstream>

#include "src/prime.hpp"
#include "src/setDistribution.hpp"
#include "src/utils.hpp"

using namespace std;

#define OVERSAMPLING_FACTOR 5

int iNumPrime = 0;

void CSV_maker() {

    char methods[2] = {'r', 'i'}; // Methods to evaluate (r for recursive, i for iterative)

    // Create the CSV file
    createCSV();

    for (int M = 1; M < 11; M++) {
        // Iterate over the range to evaluate (10^i, i = 1, 2, ...)

        cout << "M = " << M << endl;

        for (int k = 1; k < 5; k++) {
            long long N = pow(10, k); // Range to evaluate (1 to N)

            cout << "N = " << N << endl;

            // Create the elements and threadSize arrays
            long long elements[N] = {0}; // Array to store the numbers to evaluate (from 1 to N, by default)
            long long threadSize[N] = {0}; // Array to store the thread size (number of elements to evaluate in each thread)
            bool isPrime[N] = {false}; // Array to store the prime numbers

            // Fill the elements array
            for (long long i = 0; i < N; i++) {
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

                // Iterate over the methods (recursive and iterative)
                for (char method : methods) {

                    cout << "Method " << method << endl;

                    double durations[OVERSAMPLING_FACTOR] = {0}; // Array to store the duration of each oversampling factor

                    // Iterate over the oversampling factor
                    for (int i = 1; i <= OVERSAMPLING_FACTOR; i++) {
                        double durationPerThread[M] = {0}; // Array to store the duration of each thread

                        cout << "Oversampling factor " << i << endl;

                        // Give work to each thread
                        durations[i-1] = giveThreadsWork(N, M, elements, threadSize, isPrime, method, durationPerThread);

                        // cout << "\nThere are " << iNumPrime << " prime numbers" << endl;

                        for (long long i = 0; i < M; i++) {
                            threadSize[i] = 0;
                        }
                    }

                    // Calculate the average duration
                    double avgDuration = accumulate(durations, durations + OVERSAMPLING_FACTOR, 0.0) / OVERSAMPLING_FACTOR;

                    appendCSV(N, M, approach, method, 0, avgDuration);
                }

                // Restart the counter
                iNumPrime = 0;
                
                // Restart the elements and threadSize arrays
                for (long long i = 0; i < N; i++) {
                    elements[i] = i + 1;
                }
            }
        }
    }   
}

int main() {

    const int NUMTHREADS = 7;
    long long N = pow(10, 2); // Range to evaluate (1 to N)

    // Create the elements and threadSize arrays
    long long elements[N] = {0}; // Array to store the numbers to evaluate (from 1 to N, by default)
    long long threadSize[N] = {0}; // Array to store the thread size (number of elements to evaluate in each thread)
    bool isPrime[N] = {false}; // Array to store the prime numbers

    // Fill the elements array
    for (long long i = 0; i < N; i++) {
        elements[i] = i + 1;
    }

    // Uses the "card distribution" approach to split the elements in 7 (=NUMTHREADS) threads
    cardDistribution(N, NUMTHREADS, elements, threadSize);

    // Print elements of each thread
    printSets(N, NUMTHREADS, elements, threadSize);

    // Execute the approach with the iterative method
    char method = 'i';
    double durationPerThread[NUMTHREADS] = {0}; // Array to store the duration of each thread
    double duration = giveThreadsWork(N, NUMTHREADS, elements, threadSize, isPrime, method, durationPerThread);

    // Print the duration of each thread
    for (long long i = 0; i < NUMTHREADS; i++) {
        cout << "Thread " << i + 1 << " duration: " << durationPerThread[i] << " ns" << endl;
    }

    // CSV_maker();

    cout << endl;
    cout << "Threads: " << NUMTHREADS << endl;
    cout << "Total execution time (ns): " << duration << endl;
    cout << "Total numbers evaluated: " << N <<endl;
    cout << "Quantity of primes found: " << iNumPrime << endl;

    ofstream outputFile("results.txt");

    if (outputFile.is_open()) {

        outputFile << "Primes: ";
        for (unsigned int i = 0; i < N; i++) {
            if(isPrime[i] == 1) outputFile << elements[i] << "; ";
        }
        cout << endl;
        outputFile.close();
    } else {
        cerr << "Error opening results.txt" << endl;
    }


    return 0;
}

// The line to compile and exec
// g++ -std=c++11 main.cpp -o main; ./main 


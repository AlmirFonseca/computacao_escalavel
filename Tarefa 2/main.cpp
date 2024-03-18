#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>
#include <fstream>
#include <cstdlib> // For std::stoi

using namespace std;

#include "src/prime.hpp"
#include "src/setDistribution.hpp"
#include "src/utils.hpp"

int iNumPrime = 0; // Declare and initialize iNumPrime variable

// Include your function prototypes here
void CSVmaker(int N, int M, char method, int approach);

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <N> <M> <method> <approach>\n";
        return 1;
    }

    int N = std::stoi(argv[1]);
    int M = std::stoi(argv[2]);
    char method = argv[3][0];
    int approach = std::stoi(argv[4]);

    // Ensure method is either 'r' or 'i'
    if (method != 'r' && method != 'i') {
        std::cerr << "Method must be 'r' for recursive or 'i' for iterative\n";
        return 1;
    }

    // Ensure approach is within the correct range
    if (approach < 1 || approach > 5) {
        std::cerr << "Approach must be between 1 and 5\n";
        return 1;
    }

    // Now, call the function to perform a single run with the provided parameters
    CSVmaker(N, M, method, approach);

    return 0;
}

void CSVmaker(int N, int M, char method, int approach) {
    // Dynamically allocate arrays to avoid stack overflow
    auto elements = new unsigned long long int[N];
    auto threadSize = new unsigned long long int[N]; // Note: This might be better as new unsigned long long int[M] if M is the number of threads
    auto isPrime = new bool[N];
    auto durationPerThread = new double[M];

    // Initialize arrays
    for (int i = 0; i < N; ++i) {
        elements[i] = i + 1;
        isPrime[i] = false;
    }
    for (unsigned long long int i = 0; i < M; ++i) {
        threadSize[i] = 0;
    }
    for (int i = 0; i < M; ++i) {
        durationPerThread[i] = 0.0;
    }

    // Apply the chosen approach to distribute work among threads
    switch (approach) {
        case 1:
            equalSplit(N, M, elements, threadSize);
            break;
        case 2:
            shuffleAndSplit(N, M, elements, threadSize);
            break;
        case 3:
            cardDistribution(N, M, elements, threadSize);
            break;
        case 4:
            splitAndShuffle(N, M, elements, threadSize);
            break;
        case 5:
            workloadBalance(N, M, elements, threadSize);
            break;
        default:
            std::cerr << "Invalid approach: " << approach << std::endl;
            return;
    }

    // Perform the work and measure durations
    double totalDuration = giveThreadsWork(N, M, elements, threadSize, isPrime, durationPerThread, method);

    // Append the results to the CSV file
    appendCSV(N, M, approach, method, iNumPrime, totalDuration, durationPerThread);

    cout << "Threads: " << M << endl;
    cout << "Total execution time (ms): " << totalDuration << endl;
    cout << "Total numbers evaluated: " << N <<endl;
    cout << "Quantity of primes found: " << iNumPrime << endl;
    cout << "Primes: ";
        for (unsigned int i = 0; i < N; i++) {
            if(isPrime[i] == 1) cout << elements[i] << "; ";
        }
        cout << endl;
    cout << endl;

    // Clean up dynamic allocations
    delete[] elements;
    delete[] threadSize;
    delete[] isPrime;
    delete[] durationPerThread;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

extern int iNumPrime;
mutex mtx;

void printSets(unsigned long long int N, unsigned long long int M, unsigned long long int elements[], unsigned long long int threadSize[], bool printElements=true) {
    
    unsigned long long int previousThreads = 0;

    for (unsigned long long int i = 0; i < M; i++) {
        // Print the elements of eachs set
        cout << "Set " << i + 1 << " (size " << threadSize[i] << "): ";

        if (printElements) {
            for (unsigned long long int j = 0; j < threadSize[i]; j++) {
                cout << elements[previousThreads + j] << " ";
            }
        }
        cout << endl;

        previousThreads += threadSize[i];
    }
}

void updateCounter()
{
    mtx.lock();
    iNumPrime++;
    mtx.unlock();
}

void executeApproach(unsigned long long int elements[], unsigned long long int threadSize[], int idxThread, bool isPrime[], double &duration, char aproach) 
{
    // Start the clock
    auto start = chrono::steady_clock::now();

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
        if (isPrime[i]) updateCounter();
    }

    // Stop the clock
    auto end = chrono::steady_clock::now();

    // Calculate the duration
    duration = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end - start).count()) * 1e-6; // in milliseconds
}


double giveThreadsWork(unsigned long long int N, unsigned long long int M, unsigned long long int elements[], unsigned long long int threadSize[], bool isPrime[], double* durationPerThread, char method='i') {

    // Start the clock
    auto start = chrono::high_resolution_clock::now();

    // Create the threads
    thread threads[M];

    // Give work to each thread
    for (unsigned long long int i = 0; i < M; i++) {
        threads[i] = thread(executeApproach, elements, threadSize, i, ref(isPrime), ref(durationPerThread[i]), method);
    }

    // Join the threads
    for (unsigned long long int i = 0; i < M; i++) {
        threads[i].join();
    }

    // Stop the clock
    auto end = chrono::high_resolution_clock::now();

    // Calculate the duration and return it
    
    return static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end - start).count()) * 1e-6; // in milliseconds 
}

void appendCSV(unsigned long long int N, unsigned long long int M, unsigned long long int aproach, char method, unsigned long long int numPrime, double durationTotal, double durationPerThread[]) {
    // Open the file (if there is no file, it will be created; if there is a file, it will be erased)
    ofstream file("results.csv", ios::app);

    // Check if the file was opened
    if (!file.is_open()) {
        cout << "Error: the file could not be opened" << endl;
        return;
    }

    // Concatenate the durations per thread into a json string
    string durationPerThreadStr = "[";
    for (unsigned long long int i = 0; i < M; i++) {
        durationPerThreadStr += to_string(durationPerThread[i]);
        if (i < M - 1) {
            durationPerThreadStr += ";";
        }
    }
    durationPerThreadStr += "]";

    // Write the results to the file
    file << N << "," << M << "," << aproach << "," << method << "," << numPrime << "," << durationTotal << "," << durationPerThreadStr << endl;

    // Close the file
    file.close();
}

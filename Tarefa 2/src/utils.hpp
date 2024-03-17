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

void printSets(long long N, long long M, long long elements[], long long threadSize[], bool printElements=true) {
    
    long long previousThreads = 0;

    for (long long i = 0; i < M; i++) {
        // Print the elements of eachs set
        cout << "Set " << i + 1 << " (size " << threadSize[i] << "): ";

        if (printElements) {
            for (long long j = 0; j < threadSize[i]; j++) {
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

void executeApproach(long long elements[], long long threadSize[], int idxThread, bool isPrime[], double &duration, char method) 
{
    // Start the clock
    auto start = chrono::high_resolution_clock::now();

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
        if (method == 'i') {
            isPrime[i] = is_prime_it(elements[i], operations);
        } 
        else if (method == 'r') {
            isPrime[i] = is_prime_re(elements[i], operations);
        }
        if (isPrime[i]) updateCounter();
    }

    // Stop the clock
    auto end = chrono::high_resolution_clock::now();

    // Calculate the duration
    duration = static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end - start).count()); // in milliseconds
}


double giveThreadsWork(long long N, long long M, long long elements[], long long threadSize[], bool isPrime[], char method, double durationPerThread[]) {
    // Start the clock
    auto start = chrono::high_resolution_clock::now();

    // Create the threads
    thread threads[M];

    // Give work to each thread
    for (long long i = 0; i < M; i++) {
        threads[i] = thread(executeApproach, elements, threadSize, i, ref(isPrime), ref(durationPerThread[i]), method);
    }

    // Join the threads
    for (long long i = 0; i < M; i++) {
        threads[i].join();
    }

    // Stop the clock
    auto end = chrono::high_resolution_clock::now();

    // Calculate the duration and return it
    return static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(end - start).count()) * 1e-6; // in milliseconds 
}

void createCSV(long long M=1) {
    // Open the file (if there is no file, it will be created; if there is a file, it will be erased)
    ofstream file("results.csv", ios::trunc);

    // Check if the file was opened
    if (!file.is_open()) {
        cout << "Error: the file could not be opened" << endl;
        return;
    }

    // Create csv header: M, N, approach, method, totalDuration(ms), numberPrimes, duration1, duration2, ..., durationM
    file << "M,N,approach,method,totalDuration(ms),numberPrimes,";
    // for (long long i = 0; i < M; i++) {
    //     file << "duration" << i << ",";
    // }
    file << endl;

    // Close the file
    file.close();
}

void appendCSV(long long N, long long M, long long aproach, char method, double durationPerThread[], double durationTotal) {
    // Open the file (if there is no file, it will be created; if there is a file, it will be erased)
    ofstream file("results.csv", ios::app);

    // Check if the file was opened
    if (!file.is_open()) {
        cout << "Error: the file could not be opened" << endl;
        return;
    }

    // Write the results to the file
    file << M << "," << N << "," << aproach << "," << method << "," << durationTotal << "," << iNumPrime << ",";
    // for (long long i = 0; i < M; i++) {
    //     file << durationPerThread[i] << ",";
    // }
    file << endl;

    // Close the file
    file.close();
}

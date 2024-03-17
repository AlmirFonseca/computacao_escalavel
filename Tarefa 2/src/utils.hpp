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

void printSets(unsigned int N, unsigned int M, unsigned int elements[], unsigned int threadSize[], bool printElements=true) {
    
    unsigned int previousThreads = 0;

    for (unsigned int i = 0; i < M; i++) {
        // Print the elements of eachs set
        cout << "Set " << i + 1 << " (size " << threadSize[i] << "): ";

        if (printElements) {
            for (unsigned int j = 0; j < threadSize[i]; j++) {
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

void executeApproach(unsigned int elements[], unsigned int threadSize[], int idxThread, bool isPrime[], double &duration, char aproach) 
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


void giveThreadsWork(unsigned int N, unsigned int M, unsigned int elements[], unsigned int threadSize[], bool isPrime[], char aproach='i') {
    // Create the threads
    thread threads[M];

    // Array to store the duration of each thread
    double duration[M];

    // Give work to each thread
    for (unsigned int i = 0; i < M; i++) {
        threads[i] = thread(executeApproach, elements, threadSize, i, ref(isPrime), ref(duration[i]), aproach);
    }

    // Join the threads
    for (unsigned int i = 0; i < M; i++) {
        threads[i].join();
    }

    // Print the duration of each thread
    for (unsigned int i = 0; i < M; i++) {
        cout << "Thread " << i + 1 << " duration: " << duration[i] << " ms" << endl;
    }

}

void createCSV(unsigned int M) {
    // Open the file (if there is no file, it will be created; if there is a file, it will be erased)
    ofstream file("results.csv", ios::trunc);

    // Check if the file was opened
    if (!file.is_open()) {
        cout << "Error: the file could not be opened" << endl;
        return;
    }

    // Create csv header: N, M, approach, totalDuration, iNumPrime, durationPerThread[M]
    file << "N,M,approach,totalDuration,iNumPrime,";
    for (unsigned int i = 0; i < M; i++) {
        file << "duration" << i << ",";
    }

    // Close the file
    file.close();
}

void appendCSV(unsigned int N, unsigned int M, unsigned int elements[], unsigned int threadSize[], bool isPrime[], double duration, char aproach, double durationPerThread[]) {
    // Open the file (if there is no file, it will be created; if there is a file, it will be erased)
    ofstream file("results.csv", ios::app);

    // Check if the file was opened
    if (!file.is_open()) {
        cout << "Error: the file could not be opened" << endl;
        return;
    }

    // Write the results to the file
    file << N << "," << M << "," << aproach << "," << duration << "," << iNumPrime << ",";
    for (unsigned int i = 0; i < M; i++) {
        file << durationPerThread[i] << ",";
    }

    // Close the file
    file.close();
}

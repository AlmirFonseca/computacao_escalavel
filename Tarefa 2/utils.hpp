#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>

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
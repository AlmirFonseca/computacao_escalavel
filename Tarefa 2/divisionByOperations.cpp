#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <numeric>

using namespace std;

// Approach: Balance using workload categorization based on squares
void balanceUsingWorkload(int N, int M, int* elements, int* workload, pair<int, int>* sets) {
    long long totalWorkload = 0;
    
    int sqrtN = (int) sqrt(N);

    int squares[sqrtN + 1] = {0};

    // Pre-calculate workload categories
    for (int i = 0; i <= sqrt(N); ++i) {
        int start = i*i;
        int end = (i+1)*(i+1) - 1;
        end = min(end, N); // Ensure we don't go beyond N

        // Evaluate the workload for this category
        for (int j = start; j <= end; ++j) {
            totalWorkload += i;
            workload[j] = i;
        }
    }

    // Calculate target workload per set
    long long targetWorkload = totalWorkload / M;

    int currentSet = 0;
    long long currentSetWorkload = 0;

    for (int i = 0; i < N; ++i) {
        currentSetWorkload += workload[i];

        // Update the current set end index
        sets[currentSet].second = i;

        if (currentSetWorkload > targetWorkload && currentSet < M - 1) {

            // Move to the next set
            currentSet++;
            currentSetWorkload = 0;
            sets[currentSet].first = i + 1;
        
        } else {
        }
    }
}

// Utility function to print sets and workloads
void printSetsAndWorkloads(int N, int M, pair<int, int>* sets, int* workloads) {
    for (int i = 0; i < M; i++) {
        cout << "Set " << i + 1 << ": ";
        cout << "Number of elements: " << sets[i].second - sets[i].first + 1 << " (from " << sets[i].first + 1 << " to " << sets[i].second + 1 << ")";
        cout << " (Workload: " << accumulate(workloads + sets[i].first, workloads + sets[i].second + 1, 0) << ")\n";
    }
}

int main() {

    int N = 1e5; // Number of elements to distribute
    int M = 10; // Number of sets to distribute the elements into

    int elements[N] = {0};
    for (int i = 1; i <= N; i++) {
        elements[i] = i;
    }

    int workloads[N] = {0};

    pair<int, int>* sets = new pair<int, int>[M]{make_pair(0, 0)};

    cout << "Balancing " << N << " elements into " << M << " sets using workload categorization based on squares...\n\n" << endl;

    
    auto start = chrono::high_resolution_clock::now();
    
    balanceUsingWorkload(N, M, elements, workloads, sets);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;

    cout << "Execution time: " << elapsed.count() << " ms\n\n" << endl;
    
    printSetsAndWorkloads(N, M, sets, workloads);

    return 0;
}
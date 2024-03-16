#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// function that checks if a number is prime (iteratively)
bool is_prime_it(int n, int& operations) {
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        operations++;
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

// function that checks if a number is prime (recursively)
bool is_prime_re(int n, int i, int& operations) {
    if (n <= 1) {
        return false;
    }

    if (i * i > n) {
        return true;
    }

    operations++;
    if (n % i == 0) {
        return false;
    }

    return is_prime_re(n, i + 1, operations);
}


void split_numbers(int max, int numThreads, vector<int> numbers[]) {

    // Distribute the numbers in the threads
    for (int i = 1; i < max + 1; i++) {
        numbers[i % numThreads].push_back(i);
    }

    // Print the numbers in each thread
    for (int i = 0; i < numThreads; i++) {
        cout << "Thread " << i+1 << ": ";
        for (int j = 0; j < numbers[i].size(); j++) {
            cout << numbers[i][j] << " ";
        }
        cout << endl;
    }
}

// Function given to the threads
void check_prime_numbers(vector<int> numbers, bool results[], int& operations) {
    for (int i = 0; i < numbers.size(); i++) {
        results[numbers[i]] = is_prime_it(numbers[i], operations);
    }
}


int main() {
    
    int operations = 0;
    int numThreads = 11;
    int max = 100;

    // Array of vectors to hold the numbers for each thread
    vector<int> numbers[numThreads];

    // Boolean array to hold the results of the prime number check
    bool results[max];

    split_numbers(max, numThreads, numbers);

    // // Create threads
    // thread threads[numThreads];

    // // Start the threads
    // for (int i = 0; i < numThreads; i++) {
    //     threads[i] = thread(check_prime_numbers, numbers[i], results, ref(operations));
    // }

    // // Join the threads
    // for (int i = 0; i < numThreads; i++) {
    //     threads[i].join();
    // }

    // // Print the results
    // for (int i = 0; i < max; i++) {
    //     cout << i << " is prime: " << results[i] << endl;
    // }

    return 0;
}
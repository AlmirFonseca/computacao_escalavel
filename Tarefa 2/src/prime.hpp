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
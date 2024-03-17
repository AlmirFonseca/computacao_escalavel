#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {

    // Number of elements to shuffle
    const int NUM_MAX = argc < 2 ? 1 : atoi(argv[1]);

    vector<int> indices(NUM_MAX);
    for (int i = 0; i < NUM_MAX; ++i) {
        indices[i] = i;
    }

    random_device rd;
    mt19937 g(rd()); // Mersenne Twister engine for good randomness

    shuffle(indices.begin(), indices.end(), g);

    vector<bool> shuffled_numbers(NUM_MAX);
    for (int i = 0; i < NUM_MAX; ++i) {
        shuffled_numbers[i] = false;
    }

    for (int i = 0; i < NUM_MAX; ++i) {
        cout << "index (N-1): " << indices[i] << ": " << shuffled_numbers[indices[i]] << endl;
    }

    return 0;
}

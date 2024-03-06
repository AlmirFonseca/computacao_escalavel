#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

// Number of threads to use
int NUM_THREADS = 10;

string TEXT_FILE_PATH = "./shakespeare.txt"; // Path to the text file
const string LOVE = "love"; 
const string HATE = "hate";
string line; // String to hold each line of the text file

int count_word_per_memory_block(const string& line, const string& word) {
    int total = 0;

    // count how many times the word appears in the main
    size_t pos = line.find(word, 0);
    while (pos != string::npos) {
        total++;
        pos = line.find(word, pos + 1);
    }

    return total;
}

// function that returns a pair of integers that represent the number of times the word "love" and "hate" appear in the text
void count_word(const string& line, int& loveCount, int& hateCount) {
    loveCount = count_word_per_memory_block(line, LOVE);
    hateCount = count_word_per_memory_block(line, HATE);
}

int main() {

    ifstream textFile (TEXT_FILE_PATH);

    int lineCount;

    while (getline(textFile, line)) {
        lineCount++;
    }

    int linesPerThread = lineCount / NUM_THREADS;
    int remainingLines = lineCount % NUM_THREADS;

    vector<string> memoryBlocks;

    // Reset file pointer to beginning
    textFile.clear();
    textFile.seekg(0);

    for (int threadIndex = 0; threadIndex < NUM_THREADS; threadIndex++) {

        int currentThreadLineCount = 0;

        // Assign lines to memory block
        if (remainingLines > 0) {
            currentThreadLineCount = linesPerThread + 1;
            remainingLines--;
        } else {
            currentThreadLineCount = linesPerThread;
        }

        string memoryBlock = "";

        for(int i = 0; i < currentThreadLineCount; i++){
            getline(textFile, line);
            memoryBlock += line + "\n";
        }


        memoryBlocks.push_back(memoryBlock);
    }

    thread* threads = new thread[NUM_THREADS];

    int countLove[NUM_THREADS];
    int countHate[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(count_word, memoryBlocks[i], ref(countLove[i]), ref(countHate[i]));
    }

    int totalLoveCount = 0;
    int totalHateCount = 0;

    // join the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
        totalLoveCount += countLove[i];
        totalHateCount += countHate[i];
    }

    cout << endl << "Number of threads: " << NUM_THREADS << endl;
    cout << "Number of lines of each thread (remaining are distributed to the firt ones): " << linesPerThread << endl << endl;

    cout << "The word 'love' appears " << totalLoveCount << " times in the text." << endl;
    cout << "The word 'hate' appears " << totalHateCount << " times in the text." << endl;
    cout << "The word that appears the most is: " << (totalLoveCount > totalHateCount ? "love" : "hate") << endl << endl;

    return 0;
}
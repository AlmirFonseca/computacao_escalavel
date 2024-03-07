#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// Number of threads to use
int NUM_THREADS = 16;

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

    // Measure pre-processing time
    auto start = chrono::high_resolution_clock::now();

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

    textFile.close();

    // Finish pre-processing time
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> preProcessingTime = end - start;


    // Measure processing time
    start = chrono::high_resolution_clock::now();

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

    // Finish processing time
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> processingTime = end - start;


    // Threads information
    cout << endl << "Número de threads: " << NUM_THREADS << endl;
    cout << "Número de linhas por thread (as restantes são distribuídas pelas primeiras threads): " << linesPerThread << endl << endl;

    // Time information
    cout << "Tempo de preparação: " << preProcessingTime.count() << " s" << endl;
    cout << "Tempo de execução da pesquisa: " << processingTime.count() << " s" << endl;
    cout << "Tempo total decorrido: " << preProcessingTime.count() + processingTime.count() << " s" << endl << endl;

    // Results
    cout << "A palavra 'love' aparece " << totalLoveCount << " vezes no texto." << endl;
    cout << "A palavra 'hate' aparece " << totalHateCount << " vezes no texto." << endl;
    cout << "A palavra que aparece mais vezes é: " << (totalLoveCount > totalHateCount ? "love" : "hate") << endl << endl;

    return 0;
}
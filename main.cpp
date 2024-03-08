#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;


string TEXT_FILE_PATH = "./shakespeare.txt"; // Path to the text file
const string LOVE = "love"; 
const string HATE = "hate";

string line; // String to hold each line of the text file


int count_word_per_memory_block(const string& memoryBlock, const string& word) {
    int total = 0;

    size_t pos = memoryBlock.find(word);

    while (pos != string::npos) {
        bool is_valid_start = (pos == 0) || !isalpha(memoryBlock[pos - 1]);

        bool is_valid_end = (pos + word.length() == memoryBlock.length()) || !isalpha(memoryBlock[pos + word.length()]);

        if (is_valid_start && is_valid_end) {
            total++;
        }

        pos = memoryBlock.find(word, pos + 1);
    }

    return total;
}

bool isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


// function that returns a pair of integers that represent the number of times the word "love" and "hate" appear in the text
void count_word(const string& memoryBlock, int& loveCount, int& hateCount) {
    loveCount = count_word_per_memory_block(memoryBlock, LOVE);
    hateCount = count_word_per_memory_block(memoryBlock, HATE);
}

int main(int argc, char* argv[]) {
    
    const int NUM_THREADS = argc < 2 ? 1 : atoi(argv[1]);
    // Measure pre-processing time
    auto start = chrono::high_resolution_clock::now();

    ifstream textFile (TEXT_FILE_PATH);

    int lineCount = 0;

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

        int currentThreadLineCount;

        // Assign lines to memory block
        if (remainingLines > 0) {
            currentThreadLineCount = linesPerThread + 1;
            remainingLines--;
        } else {
            currentThreadLineCount = linesPerThread;
        }

        string memoryBlock;

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

    auto* threads = new thread[NUM_THREADS];

    int countLove[NUM_THREADS];
    int countHate[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(count_word, ref(memoryBlocks[i]), ref(countLove[i]), ref(countHate[i]));
    }

    int totalLoveCount = 0;
    int totalHateCount = 0;

    // join the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
        totalLoveCount += countLove[i];
        totalHateCount += countHate[i];
    }

    delete[] threads;

    // Finish processing time
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> processingTime = end - start;


    // Threads information
    cout << "Numero de threads: " << NUM_THREADS << endl;
    cout << "Tamanho de bloco de cada thread = Numero de linhas por thread (as linhas restantes sao distribuidas pelas primeiras threads): " << linesPerThread << endl;

    // Time information
    cout << "Tempo de preparacao: " << preProcessingTime.count() << " s" << endl;
    cout << "Tempo de execucao da pesquisa: " << processingTime.count() << " s" << endl;
    cout << "Tempo total decorrido: " << preProcessingTime.count() + processingTime.count() << " s" << endl;

    // Results
    cout << "Quantidade de ocorrencias de 'love': " << totalLoveCount << endl;
    cout << "Quantidade de ocorrencias de 'hate': " << totalHateCount << endl;
    cout << "A palavra que aparece mais vezes eh: " << (totalLoveCount > totalHateCount ? "love" : "hate") << endl;

    return 0;
}
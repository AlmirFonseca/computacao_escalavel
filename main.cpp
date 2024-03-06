#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int NUM_THREADS = 196391/2;
//char * TEXT_FILE_PATH = "./shakespeare.txt";

string line;
int lineCount;

int main() {

    ifstream textFile ("shakespeare.txt");

    while (getline(textFile, line)) {
        lineCount++;
    }

    cout << lineCount << endl;

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

    for (int i=0; i < 20; i++){
        cout << i << ": " << memoryBlocks[i] << endl;
    }


    return 0;
}

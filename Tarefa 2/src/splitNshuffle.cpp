#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

/**
 * Calcula os índices de um array para dividi-lo em partes do mesmo tamanho
 *
 * @param iIndices array de tamanho iNumDivs + 1, onde serão armazenados os indices da divisão
 * @param iArraySize tamanho do array
 * @param iNumDivs número de divisões do array
 */
void indices(int iIndices[], int iArraySize, int iNumDivs) {
    int iElementosExtras = iArraySize % iNumDivs;
    int iFracao = iArraySize / iNumDivs;     // Tamanho de cada parte
    int iIndice = iFracao;                   // Indices referente a cada parte

    iIndices[0] = 0;

    for(int i = 1; i <= iNumDivs; i++)
    {
        if (i-1 < iElementosExtras) iIndice++;
        iIndices[i] = iIndice;
        iIndice += iFracao;
    }

}


/**
 * Embaralhar os subarray de um array
 *
 * @param array
 * @param indices
 * @param numDiv
 */
void embaralharSubarray(int array[], int indices[], int numDiv) {
    random_device rd;
    mt19937 g(rd());
    for (int i = 0; i < numDiv; i++) {
        int comeco = indices[i];
        int fim = indices[i+1]-1;
        shuffle(&array[comeco], &array[fim], g);
    }
}

int main() {
    /*
    int tamanhoArray = 95;  // Tamanho do array
    int numThreads = 10;
    int array[tamanhoArray];
    for(int i = 0; i < tamanhoArray; i++)
    {
        array[i] = i+1;
    }
    int arrayIndices[numThreads+1];

    indices(arrayIndices, tamanhoArray, numThreads);
    for(int i = 0; i < tamanhoArray; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;

    embaralharSubarray(array, arrayIndices, numThreads);
    for(int i = 0; i < tamanhoArray; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;


    // Verifica se o resultado não é nulo
    if (indices != nullptr)
    {
        cout << "Indices do array com tamanho " << tamanhoArray << " sao: ";
        // Exibe os índices
        for (int i = 0; i <= numThreads; ++i) {
            cout << arrayIndices[i] << " ";
        }
        cout << endl;
    }*/


    return 0;
}

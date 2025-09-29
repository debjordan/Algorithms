#include <iostream>
#include <vector>

void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void bubbleSortOptimized(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

// Versão com debug para mostrar as trocas
void bubbleSortDebug(std::vector<int>& arr) {
    int n = arr.size();
    int comparisons = 0;
    int swaps = 0;

    std::cout << "Array inicial: ";
    printArray(arr);

    for (int i = 0; i < n - 1; i++) {
        std::cout << "\n=== Passada " << (i + 1) << " ===" << std::endl;
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            std::cout << "Comparando " << arr[j] << " com " << arr[j + 1] << ": ";

            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swaps++;
                swapped = true;
                std::cout << "TROCA! ";
            } else {
                std::cout << "sem troca ";
            }
            printArray(arr);
        }

        if (!swapped) {
            std::cout << "Nenhuma troca na passada - array ordenado!" << std::endl;
            break;
        }
    }

    std::cout << "\nArray final: ";
    printArray(arr);
    std::cout << "Total de comparações: " << comparisons << std::endl;
    std::cout << "Total de trocas: " << swaps << std::endl;
}

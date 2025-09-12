// versao interativa e recursiva
#include <iostream>
#include <vector>

int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

int binarySearchRecursive(const std::vector<int>& arr, int target, int left, int right) {
    if (left > right) {
        return -1;
    }
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) {
        return mid;
    }
    
    if (arr[mid] < target) {
        return binarySearchRecursive(arr, target, mid + 1, right);
    } else {
        return binarySearchRecursive(arr, target, left, mid - 1);
    }
}

int main() {
    std::vector<int> arr = {2, 3, 4, 10, 40, 55, 78, 99};
    int target = 10;
    
    std::cout << "Array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    int result = binarySearch(arr, target);
    
    if (result != -1) {
        std::cout << "Elemento " << target << " encontrado no índice " << result << std::endl;
    } else {
        std::cout << "Elemento " << target << " não encontrado" << std::endl;
    }
    
    int resultRec = binarySearchRecursive(arr, target, 0, arr.size() - 1);
    std::cout << "Busca recursiva: " << (resultRec != -1 ? "encontrado no índice " + std::to_string(resultRec) : "não encontrado") << std::endl;
    
    return 0;
}

#include <iostream>
#include <algorithm>

int partition(int arr[], int p, int r);

void quick_sort(int arr[], int p, int r) {
    if (p < r) {
        int q = partition(arr, p, r);
        quick_sort(arr, p, q - 1);
        quick_sort(arr, q + 1, r);
    }
}

int partition(int arr[], int p, int r) {
    int pivot = arr[r];
    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[r]);
    return i + 1;
}


int main() {
    static const int LENGTH = 9;
    int arr[LENGTH] = {7, 4, 9, 1, 0, 2, 10, 4, 4};

    quick_sort(arr, 0, LENGTH - 1);
    for (int i = 0; i < LENGTH; i++) {
        std::cout << arr[i] << ", ";
    }
}
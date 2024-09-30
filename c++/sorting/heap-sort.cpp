#include <iostream>
#include <algorithm>

void max_heapify(int arr[], int size, int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest = i;
    if ((l < size) && (arr[l] > arr[largest])) {
        largest = l;
    }
    if ((r < size) && (arr[r] > arr[largest])) {
        largest = r;
    }
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        max_heapify(arr, size, largest);
    }
}

void build_max_heap(int arr[], int size) {
    for (int i = size / 2 - 1; i > -1; i--) {
        max_heapify(arr, size, i);
    }
}

void heap_sort(int arr[], int size) {
    build_max_heap(arr, size);
    for (int i = size - 1; i > 0; i--) {
        std::swap(arr[i], arr[0]);
        max_heapify(arr, i, 0);
    }
}

int main() {
    int arr[9] = {5, 2, 4, 7, 1, 3, 2, 6, 11};
    int size = sizeof(arr) / sizeof(arr[0]);
    heap_sort(arr, size);
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << ", ";
    }
}
#include <iostream>
#include <vector>

void insertion_sort(std::vector<int> &arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j > -1 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j -= 1;
        }
        arr[j + 1] = key;
    }
}


int main() {
    int length;
    std::vector<int> arr;
    std::cout << "The length of the array: ";
    std::cin >> length;
    std::cout << "Elements of the array: ";
    for (int i = 0; i < length; i++) {
        int ele;
        std::cin >> ele;
        arr.push_back(ele);
    }

    insertion_sort(arr);

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ", ";
    }
    return 0;
}
#include <iostream>
#include <vector>

void bubble_sort(std::vector<int> &arr) {
    for (int i = 0; i < arr.size() - 1; i ++) {
        for (int j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void optimized_bubble_sort(std::vector<int> &arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swapped = true;
            }
            if (!swapped) {
                break;
            }
        }
    }
}



int main() {
    std::vector<int> arr;
    int length;
    std::cout << "The length of the array: " << std::endl;
    std::cin >> length;
    std::cout << "Elements of the array: ";
    for (int i = 0; i < length; i++) {
        int ele;
        std::cin >> ele;
        arr.push_back(ele);
    }

    bubble_sort(arr);

    optimized_bubble_sort(arr);

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ", ";
    }
    return 0;
}
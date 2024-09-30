#include <iostream>
#include <vector>

void merge(int arr[], int p, int q, int r);

void merge_sort(int arr[], int p, int r) {
    if (p < r) {
        int q = (p + r) / 2;
        merge_sort(arr, p, q);
        merge_sort(arr, q + 1, r);
        merge(arr, p, q, r);
    }
}

void merge(int arr[], int p, int q, int r)
{   
    std::vector<int> L, R;
    for (int i = p; i < q + 1; i++)
    {
        L.push_back(arr[i]);
    }
    L.push_back(INT_MAX);
    for (int j = q + 1; j < r + 1; j++) {
        R.push_back(arr[j]);
    }
    R.push_back(INT_MAX);
    int i = 0, j = 0;
    for (int k = p; k < r + 1; k++) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
    }
}

int main() {
    int arr[9] = {5, 2, 4, 7, 1, 3, 2, 6, 11};
    int length = sizeof(arr) / sizeof(arr[0]);
    merge_sort(arr, 0, length - 1);
    for (int i = 0; i < length; i++) {
        std::cout << arr[i] << " ";
    }
}
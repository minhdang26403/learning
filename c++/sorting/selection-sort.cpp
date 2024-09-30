#include <iostream>
#include <vector>
using namespace std;

void selectionSort(vector<int> &list) {
    int n = list.size();
    for (int i = 0; i < n; i++) {
        int pos = i;
        for (int j = i + 1; j < n; j++) {
            if (list[j] < list[pos]) {
                pos = j;
            }
        }
        int temp = list[i];
        list[i] = list[pos];
        list[pos] = list[i];
    }
}

int main() {
    return 1;
}
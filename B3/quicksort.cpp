#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

auto quicksortPartition = [](std::vector<int>& arr, int low, int high) -> int {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
};

auto quickSort = [](std::vector<int> arr) -> std::vector<int> {
    std::function<void(int, int)> sortHelper = [&](int low, int high) {
        if (low < high) {
            int pi = quicksortPartition(arr, low, high);

            sortHelper(low, pi - 1);
            sortHelper(pi + 1, high);
        }
    };

    sortHelper(0, arr.size() - 1);
    return arr;
};

/*
    TESTS
*/
TEST_CASE("Partitioning") {
    vector<int> arr = {10, 80, 30, 90, 40, 50, 70};
    int pivotIndex = quicksortPartition(arr, 0, arr.size() - 1);
    
    CHECK(arr[pivotIndex] == 70);
    CHECK(arr[0] <= 70);
    CHECK(arr[6] >= 70);
}

TEST_CASE("Quick sort") {
    vector<int> input = {64, 34, 25, 12, 22, 11, 90};
    vector<int> sortedInput = quickSort(input);
    vector<int> expected = {11, 12, 22, 25, 34, 64, 90};

    CHECK(sortedInput == expected);

    input = {1, 2, 3, 4, 5};
    sortedInput = quickSort(input);
    CHECK(sortedInput == input);

    input = {5, 4, 3, 2, 1};
    sortedInput = quickSort(input);
    expected = {1, 2, 3, 4, 5};
    CHECK(sortedInput == expected);
}

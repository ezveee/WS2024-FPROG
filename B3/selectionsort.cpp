#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

auto findMin = [](const vector<int>& arr, vector<int>::size_type start) -> vector<int>::size_type {
    auto minIdx = start;
    for (vector<int>::size_type j = start + 1; j < arr.size(); ++j) {
        if (arr[j] < arr[minIdx]) {
            minIdx = j;
        }
    }
    return minIdx;
};

auto selectionPass = [](const vector<int>& input) -> pair<vector<int>, bool> {
    vector<int> values = input;
    auto n = values.size();
    bool swapped = false;

    for (vector<int>::size_type i = 0; i < n - 1; ++i) {
        auto minIdx = findMin(values, i);
        if (minIdx != i) {
            swap(values[i], values[minIdx]);
            swapped = true;
        }
    }
    return make_pair(values, swapped);
};

auto selectionSort = [](const vector<int>& input) -> vector<int> {
    vector<int> sortedValues = input;

    for (vector<int>::size_type i = 0; i < sortedValues.size() - 1; ++i) {
        auto [values, swapped] = selectionPass(sortedValues);
        sortedValues = values;
        if (!swapped) {
            break;
        }
    }

    return sortedValues;
};

/*
    TESTS
*/
TEST_CASE("Find minimum") {
    vector<int> input = {5, 2, 4, 1, 3};
    CHECK(findMin(input, 0) == 3);
    CHECK(findMin(input, 2) == 3);
    CHECK(findMin(input, 4) == 4);
}

TEST_CASE("Selection pass") {
    vector<int> input = {3, 1, 2, 4};
    auto [result, swapped] = selectionPass(input);
    CHECK(result == vector<int>{1, 2, 3, 4});
    CHECK(swapped == true);

    input = {1, 2, 3, 4};
    auto [result2, swapped2] = selectionPass(input);
    CHECK(result2 == vector<int>{1, 2, 3, 4});
    CHECK(swapped2 == false);
}

TEST_CASE("Selection sort") {
    vector<int> input = {64, 34, 25, 12, 22, 11, 90};
    vector<int> sortedInput = selectionSort(input);
    vector<int> expected = {11, 12, 22, 25, 34, 64, 90};
    
    CHECK(sortedInput == expected);

    input = {1, 2, 3, 4, 5};
    sortedInput = selectionSort(input);
    CHECK(sortedInput == input);

    input = {5, 4, 3, 2, 1};
    sortedInput = selectionSort(input);
    expected = {1, 2, 3, 4, 5};
    CHECK(sortedInput == expected);
}

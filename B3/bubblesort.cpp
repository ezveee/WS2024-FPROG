#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

auto compareAndSwap = [](const int a, const int b) -> pair<int, int> {
    return (a > b) ? make_pair(b, a) : make_pair(a, b);
};

auto bubblePass = [](const vector<int>& input) -> pair<vector<int>, bool> {
    int size = input.size();
    vector<int> values = input;
    bool swapped = false;

    for (int i = 0; i < size - 1; ++i) {
        auto swappedPair = compareAndSwap(values[i], values[i + 1]);
        if (swappedPair.first != values[i] || swappedPair.second != values[i + 1]) {
            swapped = true;
        }
        values[i] = swappedPair.first;
        values[i + 1] = swappedPair.second;
    }

    return make_pair(values, swapped);
};

auto bubbleSort = [](const vector<int>& input) -> vector<int> {
    int size = input.size();
    vector<int> sortedValues = input;

    for (int i = 0; i < size - 1; ++i) {
        auto [values, swapped] = bubblePass(sortedValues);
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
TEST_CASE("Compare and swap") {
    CHECK(compareAndSwap(2, 1) == make_pair(1, 2));
    CHECK(compareAndSwap(1, 2) == make_pair(1, 2));
    CHECK(compareAndSwap(5, 5) == make_pair(5, 5));
}

TEST_CASE("Bubble pass") {
    vector<int> input = {3, 1, 2, 4};
    auto [result, swapped] = bubblePass(input);
    CHECK(result == vector<int>{1, 2, 3, 4});
    CHECK(swapped == true);

    input = {1, 2, 3, 4};
    auto [result2, swapped2] = bubblePass(input);
    CHECK(result2 == vector<int>{1, 2, 3, 4});
    CHECK(swapped2 == false);
}

TEST_CASE("Bubble sort") {
    vector<int> input = {64, 34, 25, 12, 22, 11, 90};
    vector<int> sortedInput = bubbleSort(input);
    vector<int> expected = {11, 12, 22, 25, 34, 64, 90};
    
    CHECK(sortedInput == expected);

    input = {1, 2, 3, 4, 5};
    sortedInput = bubbleSort(input);
    CHECK(sortedInput == input);

    input = {5, 4, 3, 2, 1};
    sortedInput = bubbleSort(input);
    expected = {1, 2, 3, 4, 5};
    CHECK(sortedInput == expected);
}
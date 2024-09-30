#include <iostream>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;
using Bucket = vector<float>;

auto determineBucketIndex = [](const float value, const int listSize) -> int {
    return value * listSize;
};

auto assignBuckets = [](const vector<float>& arr, int size) {
    vector<Bucket> buckets(size);
    for (const float& value : arr) {
        int index = determineBucketIndex(value, size);
        buckets[index].push_back(value);
    }
    return buckets;
};

auto sortIndividualBucket = [](const Bucket& inputBucket) {
    Bucket outputBucket = inputBucket;
    sort(outputBucket.begin(), outputBucket.end());
    return outputBucket;
};

auto sortAllBuckets = [](const vector<Bucket>& inputBuckets) {
    vector<Bucket> outputBuckets = inputBuckets;
    for (size_t i = 0; i < outputBuckets.size(); ++i) {
        outputBuckets[i] = sortIndividualBucket(outputBuckets[i]);
    }
    return outputBuckets;
};

auto flattenBucketsIntoVector = [](const vector<Bucket>& inputBuckets) {
    vector<Bucket> buckets = inputBuckets;
    vector<float> sortedArray;
    for (const Bucket& bucket : buckets) {
        for (const float& value : bucket) {
            sortedArray.push_back(value);
        }
    }
    return sortedArray;
};

auto bucketSort = [](const vector<float>& toBeSorted, int size) {
    return flattenBucketsIntoVector(
        sortAllBuckets(
            assignBuckets(toBeSorted, size)
        )
    );
};

TEST_CASE("Determine bucket index") {
    // INPUT: float value, array size
    // TASK: determines where in the buckets container the value should be inserted
    // OUTPUT: int -> index for insertion

    float f = 0.69783f;
    int listSize = 7;
    int expected = 4;
    
    int result = determineBucketIndex(f, listSize);
    CHECK_EQ(expected, result);
}

TEST_CASE("Assign buckets") {
    // INPUT: float array of size n
    // TASK: assigns elements into corresponding bucket
    // OUTPUT: bucket -> array of float vectors
    
    vector<float> arr = {0.897, 0.565, 0.656, 0.1234, 0.665, 0.3434};

    vector<Bucket> expected = {
        {0.1234},
        {},
        {0.3434},
        {0.565, 0.656, 0.665},
        {},
        {0.897}
    };

    vector<Bucket> result = assignBuckets(arr, arr.size());
    CHECK_EQ(result, expected);
}

TEST_CASE("Sort individual bucket") {
    // INPUT: bucket -> vector of floats
    // TASK: sorts all elements in bucket
    // OUTPUT: sorted bucket

    Bucket input = {0.69, 0.3216, 0.54};

    Bucket expected = {0.3216, 0.54, 0.69};

    Bucket result = sortIndividualBucket(input);

    CHECK_EQ(result, expected);
}

TEST_CASE("Sort all buckets") {
    // INPUT: buckets -> vector of vectors of floats
    // TASK: sorts all elements in all buckets
    // OUTPUT: sorted buckets

    vector<Bucket> input = {
        {0.69, 0.3216, 0.54},
        {},
        {0.420},
        {},
        {0.646, 0.521},
        {}
    };

    vector<Bucket> expected = {
        {0.3216, 0.54, 0.69},
        {},
        {0.420},
        {},
        {0.521, 0.646},
        {}
    };

    vector<Bucket> result = sortAllBuckets(input);

    CHECK_EQ(result, expected);
}

TEST_CASE("Flatten buckets into vector") {
    // INPUT: buckets -> vector of vectors of floats
    // TASK: turns all buckets into one vector of floats
    // OUTPUT: sorted float vector

    vector<Bucket> input = {
        {0.3216, 0.54, 0.69},
        {},
        {0.420},
        {},
        {0.521, 0.646},
        {}
    };

    vector<float> expected = {0.3216, 0.54, 0.69, 0.420, 0.521, 0.646};

    vector<float> result = flattenBucketsIntoVector(input);

    CHECK_EQ(result, expected);
}

TEST_CASE("Bucket sort") {
    // INPUT: initial vector of floats
    // TASK: calls all necessary lambdas for bucket sort
    // OUTPUT: sorted float vector

    vector<float> input = {0.897, 0.565, 0.656, 0.1234, 0.665, 0.3434};
    
    vector<float> expected = {0.1234, 0.3434, 0.565, 0.656, 0.665, 0.897};

    vector<float> result = bucketSort(input, input.size());
    for (auto element : result) {
        cout << element << ", ";
    }

    CHECK_EQ(result, expected);
}
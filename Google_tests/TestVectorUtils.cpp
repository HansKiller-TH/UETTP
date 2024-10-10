//
// Created by hansk on 17.05.2024.
//
#include "gtest/gtest.h"
#include "vectorUtils/VectorUtils.h"

TEST(VectorUtilsTest, IndexForValue_ValueExists_ReturnsCorrectIndex) {
    std::vector<int> testVector = {2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, 6), 2);
}

TEST(VectorUtilsTest, IndexForValue_ValueNotExists_ReturnsCorrectIndex) {
    std::vector<int> testVector = {2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, 1), -1);
}

TEST(VectorUtilsTest, IndexForValue_EmptyVector_ReturnsMinusOne) {
    std::vector<int> testVector = {};
    ASSERT_EQ(-1, VectorUtils::indexForValue(testVector, 1));
}

TEST(VectorUtilsTest, IndexForValue_RepeatedValues_ReturnsFirstOccurrence) {
    std::vector<int> testVector = {2, 4, 6, 2, 2, 10};
    ASSERT_EQ(0, VectorUtils::indexForValue(testVector, 2));
}

TEST(VectorUtilsTest, IndexForValue_NegativeAndZeroValues_ReturnsCorrectIndex) {
    std::vector<int> testVector = {-2, 0, 2, 4, 6};
    ASSERT_EQ(0, VectorUtils::indexForValue(testVector, -2));
    ASSERT_EQ(1, VectorUtils::indexForValue(testVector, 0));
}

// Passing an integer as a string
TEST(VectorUtilsTest, IndexForValue_StringInteger_ReturnsCorrectIndex) {
    std::vector<int> testVector = {2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, "6"), 2);
}

// Passing a string that can't be converted to integer
TEST(VectorUtilsTest, IndexForValue_StringNonInteger_ReturnsMinusOne) {
    std::vector<int> testVector = {2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, "abc"), -1);
}

// Passing a string that represents a negative integer
TEST(VectorUtilsTest, IndexForValue_StringNegativeInteger_ReturnsCorrectIndex) {
    std::vector<int> testVector = {-2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, "-2"), 0);
}

// Passing a string that represents a decimal number
TEST(VectorUtilsTest, IndexForValue_StringDecimal_ReturnsMinusOne) {
    std::vector<int> testVector = {2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, "3.14"), -1);
}

// Passing an empty string
TEST(VectorUtilsTest, IndexForValue_EmptyString_ReturnsMinusOne) {
    std::vector<int> testVector = {2, 4, 6, 8, 10};
    ASSERT_EQ(VectorUtils::indexForValue(testVector, ""), -1);
}

// All vectors have the same size and all integers at given index are positive.
TEST(VectorUtilsTest, FindIndexesWherePredicateAllOf_ArraysSameSizeIntegersPositive_ReturnsCorrectIndexes) {
    std::vector<std::vector<int>> testVectors = {{1, 2, 3},
                                                 {4, 5, 6},
                                                 {7, 8, 9}};
    auto predicate = [](const int &val) { return val > 0; };
    std::set<int> expectedResult = {0, 1, 2};
    ASSERT_EQ(VectorUtils::getIndexesWherePredicateAllOf(testVectors, predicate), expectedResult);
}

// All vectors have different sizes.
TEST(VectorUtilsTest, FindIndexesWherePredicateAllOf_ArraysDifferentSize_ThrowsException) {
    std::vector<std::vector<int>> testVectors = {{1, 2, 3},
                                                 {4, 5},
                                                 {6}};
    auto predicate = [](const int &val) { return val > 0; };
    ASSERT_THROW(VectorUtils::getIndexesWherePredicateAllOf(testVectors, predicate), std::runtime_error);
}

// Some values at the same indexes are negative.
TEST(VectorUtilsTest, FindIndexesWherePredicateAllOf_SomeNegativeValues_ReturnsCorrectIndexes) {
    std::vector<std::vector<int>> testVectors = {{1, 2,  -3},
                                                 {4, -5, 6},
                                                 {7, 8,  9}};
    auto predicate = [](const int &val) { return val > 0; };
    std::set<int> expectedResult = {0};
    ASSERT_EQ(VectorUtils::getIndexesWherePredicateAllOf(testVectors, predicate), expectedResult);
}

// All vectors have the same size but no values at the same indexes are positive.
TEST(VectorUtilsTest, FindIndexesWherePredicateAllOf_NoPositiveValues_ReturnsEmpty) {
    std::vector<std::vector<int>> testVectors = {{-1, -2, -3},
                                                 {-4, -5, -6},
                                                 {-7, -8, -9}};
    auto predicate = [](const int &val) { return val > 0; };
    std::set<int> expectedResult = {};
    ASSERT_EQ(VectorUtils::getIndexesWherePredicateAllOf(testVectors, predicate), expectedResult);
}

TEST(VectorUtilsTest, TestEmptyBins) {
    int itemSize = 0;
    std::vector<std::pair<int, int>> binSizes;

    auto result = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::pair<int, int> asserter{0, 0};
    ASSERT_EQ(result, asserter);
}

TEST(VectorUtilsTest, TestSumLessThanItemSize) {
    int itemSize = 10;
    std::vector<std::pair<int, int>> binSizes = {{1, 1},
                                                 {2, 2},
                                                 {3, 3}};

    auto result = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::pair<int, int> asserter{0, 6};
    ASSERT_EQ(result, asserter);
}

TEST(VectorUtilsTest, TestSumEqualToItemSize) {
    int itemSize = 6;
    std::vector<std::pair<int, int>> binSizes = {{1, 1},
                                                 {2, 2},
                                                 {3, 3}};

    auto result = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::pair<int, int> asserter{binSizes.size(), 6};
    ASSERT_EQ(result, asserter);
}

TEST(VectorUtilsTest, TestSumGreaterThanItemSize) {
    int itemSize = 7;
    std::vector<std::pair<int, int>> binSizes = {{1, 1},
                                                 {2, 2},
                                                 {3, 3},
                                                 {4, 4}};

    auto result = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::pair<int, int> asserter{2, 7};
    ASSERT_EQ(result, asserter);
}

TEST(VectorUtilsTest, TestItemSizeFitFirst) {
    int itemSize = 5;
    std::vector<std::pair<int, int>> binSizes = {{1, 5},
                                                 {2, 6},
                                                 {3, 10},
                                                 {4, 50}};
    auto numAndSumBins = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::set<int> bins;
    VectorUtils::binPackingSmallestAndLeastBins(itemSize, numAndSumBins.first, numAndSumBins.second, binSizes, bins,
                                                std::set<int>(), 0, 0);
    ASSERT_EQ(bins, std::set<int>{1});
}

TEST(VectorUtilsTest, TestItemSizeFitLast) {
    int itemSize = 50;
    std::vector<std::pair<int, int>> binSizes = {{1, 5},
                                                 {2, 6},
                                                 {3, 10},
                                                 {4, 50}};
    auto numAndSumBins = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::set<int> bins;
    VectorUtils::binPackingSmallestAndLeastBins(itemSize, numAndSumBins.first, numAndSumBins.second, binSizes, bins,
                                                std::set<int>(), 0, 0);
    ASSERT_EQ(bins, std::set<int>{4});
}

TEST(VectorUtilsTest, TestItemSizeFitMiddle) {
    int itemSize = 10;
    std::vector<std::pair<int, int>> binSizes = {{1, 5},
                                                 {2, 6},
                                                 {3, 10},
                                                 {4, 50}};
    auto numAndSumBins = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::set<int> bins;
    VectorUtils::binPackingSmallestAndLeastBins(itemSize, numAndSumBins.first, numAndSumBins.second, binSizes, bins,
                                                std::set<int>(), 0, 0);
    ASSERT_EQ(bins, std::set<int>{3});
}

TEST(VectorUtilsTest, TestItemSizeFitFirstAndLast) {
    int itemSize = 64;
    std::vector<std::pair<int, int>> binSizes = {{1, 30},
                                                 {2, 33},
                                                 {3, 33},
                                                 {4, 40}};
    auto numAndSumBins = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::set<int> bins;
    VectorUtils::binPackingSmallestAndLeastBins(itemSize, numAndSumBins.first, numAndSumBins.second, binSizes, bins,
                                                std::set<int>(), 0, 0);
    std::set<int> asserter{2, 3};
    ASSERT_EQ(bins, asserter);
}

TEST(VectorUtilsTest, TestItemSizeFitLastTwo) {
    int itemSize = 73;
    std::vector<std::pair<int, int>> binSizes = {{1, 30},
                                                 {2, 33},
                                                 {3, 33},
                                                 {4, 40}};
    auto numAndSumBins = VectorUtils::getLeastNumberAndSumOfBinsRequired(itemSize, binSizes);
    std::set<int> bins;
    VectorUtils::binPackingSmallestAndLeastBins(itemSize, numAndSumBins.first, numAndSumBins.second, binSizes, bins,
                                                std::set<int>(), 0, 0);
    std::set<int> asserter{2, 4};
    ASSERT_EQ(bins, asserter);
}

TEST(VectorUtilsTest, BinsAll) {
    std::vector<std::pair<int, int>> binIndexAndSize = {
            {7, 80},
            {6, 30},
            {5, 20},
            {4, 15},
            {3, 10},
            {2, 7},
            {1, 6},
            {0, 5}};
    auto result = VectorUtils::binPackingAllBins(40, binIndexAndSize);
    std::vector<std::pair<std::set<int>, int>> asserter = {{{7},             {80}},
                                                           {{5, 6},          {50}},
                                                           {{4, 6},          {45}},
                                                           {{3, 6},          {40}},
                                                           {{1, 2, 6},       {43}},
                                                           {{0, 2, 6},       {42}},
                                                           {{0, 1, 6},       {41}},
                                                           {{3, 4, 5},       {45}},
                                                           {{2, 4, 5},       {42}},
                                                           {{1, 4, 5},       {41}},
                                                           {{0, 4, 5},       {40}},
                                                           {{1, 2, 3, 5},    {43}},
                                                           {{0, 2, 3, 5},    {42}},
                                                           {{0, 1, 3, 5},    {41}},
                                                           {{0, 1, 2, 3, 4}, {43}}};
    ASSERT_EQ(result, asserter);
}

TEST(VectorUtilsTest, sortBinResult){
    std::vector<std::pair<int, int>> binIndexAndSize = {
            {7, 80},
            {6, 30},
            {5, 20},
            {4, 15},
            {3, 10},
            {2, 7},
            {1, 6},
            {0, 5}};
    auto result = VectorUtils::binPackingAllBins(40, binIndexAndSize);
    VectorUtils::sortBinResult(result);
    std::vector<std::pair<std::set<int>, int>> asserter = {{{7},             {80}},
                                                           {{3, 6},          {40}},
                                                           {{4, 6},          {45}},
                                                           {{5, 6},          {50}},
                                                           {{0, 4, 5},       {40}},
                                                           {{1, 4, 5},       {41}},
                                                           {{0, 1, 6},       {41}},
                                                           {{2, 4, 5},       {42}},
                                                           {{0, 2, 6},       {42}},
                                                           {{1, 2, 6},       {43}},
                                                           {{3, 4, 5},       {45}},
                                                           {{0, 1, 3, 5},    {41}},
                                                           {{0, 2, 3, 5},    {42}},
                                                           {{1, 2, 3, 5},    {43}},
                                                           {{0, 1, 2, 3, 4}, {43}}};
    ASSERT_EQ(result,asserter);
}

TEST(VectorUtilsTest, getValuesIndexSecond){
    std::set<int> indexes = {0,1,2,3};
    std::vector<int> values = {5,6,7,8};
    auto result = VectorUtils::getSortedValuesUsingValueAsIndexInSecond(indexes, values, [](std::pair<int, int> a, std::pair<int, int> b){return a.second > b.second;});
    std::vector<std::pair<int, int>> asserter = {{3,8},
                                                 {2,7},
                                                 {1,6},
                                                 {0,5}};
    ASSERT_EQ(result, asserter);
}
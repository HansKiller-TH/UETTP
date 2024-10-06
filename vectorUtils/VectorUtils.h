//
// Created by hansk on 21.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_VECTORUTILS_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_VECTORUTILS_H

#include <set>
#include <vector>
#include <functional>
#include <list>
#include <optional>

class VectorUtils {
public:
    /**@brief Returns all indexes of the vector where the predicate is true. Predicate receives value at index*/
    static std::set<int>
    getIndexesWherePredicate(const std::vector<int> &vec, const std::function<bool(const int &)> &pred);

    /**@brief Returns all indexes of the vectors where the predicate is true for all vectors. Predicate receives value at index. Vectors must be same size.*/
    static std::set<int> getIndexesWherePredicateAllOf(const std::vector<std::vector<int>> &vecs,
                                                       const std::function<bool(const int &)> &pred);

    static int indexForValue(const std::vector<int> &vec, const int &value);

    static int indexForValue(const std::vector<int> &vec, const std::string &value);

    static std::vector<std::pair<int, int>> getSortedValuesUsingValueAsIndexInSecond(
            const std::set<int> &valuesAsIndexes, const std::vector<int> &values, const std::function<bool(
            std::pair<int, int>, std::pair<int, int>)> &comparator);

    static std::pair<int, int>
    getLeastNumberAndSumOfBinsRequired(const int &itemSize, const std::vector<std::pair<int, int>> &binIndexAndSize);

    static void binPackingSmallestAndLeastBins(const int &itemSize, const int &numberOfBinsRequired, int &maxSum,
                                               const std::vector<std::pair<int, int>> &binIndexAndSize,
                                               std::set<int> &result, std::set<int> currentBins = {},
                                               int currentIndex = 0, int currentSum = 0);

    static void binPackingLeastBinsAll(const int &itemSize, const std::vector<std::pair<int, int>> &binIndexAndSize,
                                       const int &numberOfBinsRequired, std::vector<std::set<int>> &results,
                                       std::set<int> currentBins = {},
                                       int currentIndex = 0, int currentSum = 0);

    static std::vector<std::pair<std::set<int>, int>>
    binPackingAllBins(const int &itemSize, const std::vector<std::pair<int, int>> &binIndexAndSize,
                      std::set<int> currentBins = {}, int currentIndex = 0, int currentSum = 0);

    static bool customComparator(const std::pair<std::set<int>, int> &a, const std::pair<std::set<int>, int> &b);

    static void sortBinResult(std::vector<std::pair<std::set<int>, int>> &binResult);

    static std::optional<std::set<int>>
    getfirstSubset(const std::set<int> &set, const std::vector<std::pair<std::set<int>, int>> &binSetsAndSizes);

    static std::optional<std::vector<std::set<int>>>
    getSubsets(const std::set<int> &set, const std::vector<std::pair<std::set<int>, int>> &binSetsAndSizes, const int& number = 1);

    static std::set<int> getIndexesFromBinaryVector(const std::vector<int> &binaryVector, int compareValue);

    static std::vector<std::pair<std::string, std::set<std::string>>>
    inLineKeyValues(const std::vector<std::vector<std::string>> &data);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_VECTORUTILS_H

//
// Created by hansk on 21.05.2024.
//

#include <stdexcept>
#include <list>
#include <set>
#include <iostream>
#include <optional>
#include "VectorUtils.h"

std::set<int>
VectorUtils::getIndexesWherePredicate(const std::vector<int> &vec, const std::function<bool(const int &)> &pred) {
    return getIndexesWherePredicateAllOf(std::vector<std::vector<int>>{vec}, pred);
}

std::set<int>
VectorUtils::getIndexesWherePredicateAllOf(const std::vector<std::vector<int>> &vecs,
                                           const std::function<bool(const int &)> &pred) {
    for (int i = 0; i < vecs.size() - 1; ++i)
        if (vecs.at(i).size() != vecs.at(i + 1).size())
            throw std::runtime_error("Vectors have different sizes");

    std::set<int> result;
    for (int index = 0; index < vecs.front().size(); ++index)
        if (std::all_of(vecs.begin(), vecs.end(),
                        [&pred, &index](const std::vector<int> &vec) { return pred(vec.at(index)); }))
            result.insert(index);
    return result;
}

int VectorUtils::indexForValue(const std::vector<int> &vec, const int &value) {
    auto it = std::find(vec.begin(), vec.end(), value);
    if (it == vec.end())
        return -1;
    return static_cast<int>(std::distance(vec.begin(), it));
}

int VectorUtils::indexForValue(const std::vector<int> &vec, const std::string &value) {
    int val;
    try {
        val = std::stoi(value);
    }
    catch (const std::exception &e) { return -1; }
    return indexForValue(vec, val);
}

std::vector<std::pair<int, int>>
VectorUtils::getSortedValuesUsingValueAsIndexInSecond(const std::set<int> &valuesAsIndexes,
                                                      const std::vector<int> &values,
                                                      const std::function<bool(std::pair<int, int>,
                                                                               std::pair<int, int>)> &comparator) {
    std::vector<std::pair<int, int>> result;
    result.reserve(valuesAsIndexes.size());
    for (auto &value: valuesAsIndexes)
        result.emplace_back(value, values.at(value));
    std::sort(result.begin(), result.end(), comparator);
    return result;
}

void
VectorUtils::binPackingLeastBinsAll(const int &itemSize,
                                    const std::vector<std::pair<int, int>> &binIndexAndSize,
                                    const int &numberOfBinsRequired,
                                    std::vector<std::set<int>> &results,
                                    std::set<int> currentBins,
                                    int currentIndex,
                                    int currentSum) {
    if (currentSum >= itemSize) {
        results.push_back(currentBins);
        return;
    }
    if (currentBins.size() >= numberOfBinsRequired) // Pruning
        return;
    //Pruning. Beyond there aren't sufficient bins to satisfy the item size
    const auto limit = binIndexAndSize.size() - numberOfBinsRequired + currentBins.size() + 1;
    for (int i = currentIndex; i < limit; ++i) {
        currentBins.insert(binIndexAndSize.at(i).first);
        binPackingLeastBinsAll(itemSize, binIndexAndSize, numberOfBinsRequired, results, currentBins, i + 1,
                               currentSum + binIndexAndSize.at(i).second);
        currentBins.erase(binIndexAndSize.at(i).first); // Backtracking
    }
}

void
VectorUtils::binPackingSmallestAndLeastBins(const int &itemSize, const int &numberOfBinsRequired, int &maxSum,
                                            const std::vector<std::pair<int, int>> &binIndexAndSize,
                                            std::set<int> &result, std::set<int> currentBins, const int currentIndex,
                                            const int currentSum) {
    if (currentSum >= itemSize && currentSum <= maxSum) {
        maxSum = currentSum;
        result = currentBins;
    }
    if (currentBins.size() >= numberOfBinsRequired) // Pruning
        return;
    //Pruning. Beyond there aren't sufficient bins to satisfy the item size
    const auto limit = binIndexAndSize.size() - numberOfBinsRequired + currentBins.size() + 1;
    for (int i = currentIndex; i < limit; ++i) {
        currentBins.insert(binIndexAndSize.at(i).first);
        binPackingSmallestAndLeastBins(itemSize, numberOfBinsRequired, maxSum, binIndexAndSize, result, currentBins,
                                       i + 1, currentSum + binIndexAndSize.at(i).second);
        currentBins.erase(binIndexAndSize.at(i).first); // Backtracking
        // Pruning. Replacing a room with a room of equal size will not yield different sums
        if (i + 1 < limit && binIndexAndSize.at(i).second == binIndexAndSize.at(i + 1).second)
            ++i;
    }
}

std::vector<std::pair<std::set<int>, int>>
VectorUtils::binPackingAllBins(const int &itemSize, const std::vector<std::pair<int, int>> &binIndexAndSize,
                               std::set<int> currentBins, int currentIndex, int currentSum) {
    std::vector<std::pair<std::set<int>, int>> result;
    if (currentSum >= itemSize) {
        result.emplace_back(currentBins, currentSum);
        return result;
    }
    for (int i = currentIndex; i < binIndexAndSize.size(); ++i) {
        auto index = binIndexAndSize.at(i).first;
        auto size = binIndexAndSize.at(i).second;
        currentBins.insert(index);
        auto subResult = binPackingAllBins(itemSize, binIndexAndSize, currentBins, i + 1, currentSum + size);
        if (subResult.empty())
            break;
        else
            result.insert(result.end(), subResult.begin(), subResult.end());
        currentBins.erase(index);
    }
    return result;
}

bool VectorUtils::customComparator(const std::pair<std::set<int>, int> &a, const std::pair<std::set<int>, int> &b) {
    if (a.first.size() == b.first.size()) {
        if (a.second == b.second)
            return *a.first.end() - *a.first.begin() < *b.first.end() - *b.first.begin();
        return a.second < b.second;
    }
    return a.first.size() < b.first.size();
}

/**
 * @brief Sorts a vector of pairs where each pair consists of a set of integers and an integer value.
 *
 * This function sorts the vector `binResult` in place, using a custom comparator. The custom comparator
 * sorts primarily by the size of the set in ascending order. If the sizes of the sets are equal,
 * comparison is done based on the integer value in ascending order. If both the sizes of the sets and
 * the integer values are equal, then the difference between the largest and smallest elements in the
 * set is used as a tie-breaker.
 *
 * @param binResult A reference to the vector of pairs to be sorted.
 */
void VectorUtils::sortBinResult(std::vector<std::pair<std::set<int>, int>> &binResult) {
    std::sort(binResult.begin(), binResult.end(), customComparator);
}

std::pair<int, int> VectorUtils::getLeastNumberAndSumOfBinsRequired(const int &itemSize,
                                                                    const std::vector<std::pair<int, int>> &binIndexAndSize) {
    if (binIndexAndSize.empty() || itemSize == 0)
        return {0, 0};
    int numberOfBinsRequired = 0;
    int sum = 0;
    for (auto it = binIndexAndSize.rbegin(); it != binIndexAndSize.rend(); ++it) {
        sum += (*it).second;
        numberOfBinsRequired++;
        if (sum >= itemSize)
            return {numberOfBinsRequired, sum};
    }
    return {0, sum};
}

/**
 * Retrieves a specified number of subsets from the provided set that meet certain inclusion criteria.
 *
 * This method iterates over a vector of pairs containing sets and corresponding integer values.
 * For each pair, it checks if the first set of the pair is a subset of the provided set.
 * If it is, the subset is added to the result. The process continues until the specified number
 * of subsets is collected or all pairs have been checked.
 *
 * @param set The main set from which subsets are to be derived.
 * @param binSetsAndSizes A vector containing pairs of sets and their corresponding integer values.
 * @param limit The number of subsets to retrieve.
 *
 * @return An optional vector containing the subsets that meet the criteria. If no valid subsets are found, returns an empty optional.
 */
std::optional<std::vector<std::set<int>>>
VectorUtils::getSubsets(const std::set<int> &set, const std::vector<std::pair<std::set<int>, int>> &binSetsAndSizes,
                        int limit) {
    std::optional<std::vector<std::set<int>>> result;
    for (const auto &pair: binSetsAndSizes) {
        if (!std::includes(set.begin(), set.end(), pair.first.begin(), pair.first.end()))
            continue;
        if (!result.has_value())
            result = std::vector<std::set<int>>{};
        result->emplace_back(pair.first);
        if (result->size() == limit)
            return result;
    }
    return result;
}

std::set<int> VectorUtils::getIndexesFromBinaryVector(const std::vector<int> &binaryVector, int compareValue) {
    std::set<int> result;
    for (int index = 0; index < binaryVector.size(); ++index)
        if (binaryVector.at(index) == compareValue)
            result.insert(index);
    return result;
}

std::vector<std::pair<std::string, std::set<std::string>>>
VectorUtils::inLineKeyValues(const std::vector<std::vector<std::string>> &data) {
    // We use this to check if we encountered a new key
    // Setting it to a modified version of the first key ensures it will be different
    std::string prev = data.front().front() + "X";
    // In this we gather all sets before processing them
    std::vector<std::pair<std::string, std::set<std::string>>> inLined;

    // Gather all sets
    for (auto &row: data) {
        // if we come to a new key
        if (row.at(0) != prev) {
            prev = row.at(0);
            // add a new set
            inLined.emplace_back(std::pair<std::string, std::set<std::string>>(prev, {}));
        }
        // We insert the value into the set
        inLined.back().second.insert(row.at(1));
    }
    return inLined;
}

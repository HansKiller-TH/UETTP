//
// Created by hansk on 21.05.2024.
//

#include <stdexcept>
#include <list>
#include <set>
#include "VectorUtils.h"

std::list<int>
VectorUtils::getIndexesWherePredicate(const std::vector<int> &vec, const std::function<bool(const int &)> &pred) {
    std::list<int> result;
    for (int index = 0; index < vec.size(); ++index)
        if (pred(vec.at(index)))
            result.push_back(index);
    return result;
}

std::set<int>
VectorUtils::getIndexesWherePredicateAllOf(const std::vector<std::vector<int>> &vecs,
                                           const std::function<bool(const int &)> &pred) {
    for (int i = 0; i < vecs.size() - 1; ++i) {
        if (vecs.at(i).size() != vecs.at(i + 1).size())
            throw std::runtime_error("Vectors have different sizes");
    }
    std::set<int> result;
    for (int index = 0; index < vecs.front().size(); ++index) {
        bool push = std::all_of(vecs.begin(), vecs.end(),
                                [&pred, &index](const std::vector<int> &vec) { return pred(vec.at(index)); });
        if (push)
            result.insert(index);
    }
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

void
VectorUtils::binPackingLeastBinsAll(const int &itemSize, const std::vector<std::pair<int, int>> &binIndexAndSize,
                                    const int &numberOfBinsRequired, std::vector<std::set<int>> &results,
                                    std::set<int> currentBins, int currentIndex, int currentSum) {
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
        if (i + 1 < limit && binIndexAndSize.at(i ).second == binIndexAndSize.at(i + 1).second)
          ++i;
    }
}

std::pair<int, int> VectorUtils::getLeastNumberAndSumOfBinsRequired(const int &itemSize,
                                                                    const std::vector<std::pair<int, int>> &binIndexAndSize) {
    if (binIndexAndSize.empty() || itemSize == 0)
        return {0,0};
    int numberOfBinsRequired = 0;
    int sum = 0;
    for (auto it = binIndexAndSize.rbegin(); it != binIndexAndSize.rend(); ++it) {
        sum += (*it).second;
        numberOfBinsRequired++;
        if (sum >= itemSize)
            return {numberOfBinsRequired, sum};
    }
    return {0,sum};
}

std::vector<std::pair<std::string, std::set<std::string>>>
VectorUtils::inLineKeyValues(const std::vector<std::vector<std::string>>& data) {
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
            inLined.emplace_back(std::pair<std::string,std::set<std::string>> (prev,{}));
        }
        // We insert the value into the set
        inLined.back().second.insert(row.at(1));
    }
    return inLined;
}

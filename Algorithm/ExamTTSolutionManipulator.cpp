//
// Created by hansk on 21.05.2024.
//

#include <iostream>
#include <set>
#include <utility>
#include "ExamTTSolutionManipulator.h"
#include "vectorUtils/VectorUtils.h"
#include "screenOutput.h"


void ExamTTSolutionManipulator::setSolution(std::shared_ptr<ExamTTSolution> solution) {
    this->solution_ = std::move(solution);
}

std::shared_ptr<ExamTTSolution> ExamTTSolutionManipulator::getSolution() {
    return this->solution_;
}

int ExamTTSolutionManipulator::getRandomExam() {
    int max = static_cast<int>(solution_->examData->examID.size() - 1);
    std::uniform_int_distribution<> distr(0, max);
    return distr(gen);
}

std::set<int> ExamTTSolutionManipulator::getRandomExamWithConnected() {
    auto rand = getRandomExam();
    std::set<int> exams = solution_->examData->examSamePeriod.at(rand);
    exams.insert(rand);
    return exams;
}

std::pair<std::set<int>, int> ExamTTSolutionManipulator::getRandomExamWithConnectedWithPeriod() {
    auto exams = getRandomExamWithConnected();
    auto period = solution_->examPeriod.at(*(exams.begin()));
    return {exams, period};
}

std::set<int> ExamTTSolutionManipulator::removeExamWithLargestDegreeOfSaturation(std::set<int> &unscheduledExams) {
    if (unscheduledExams.empty())
        return {};
    int exam = -1;
    int maxDeg = -1;
    int maxSatDeg = -1;
    for (auto &unscheduledExam: unscheduledExams) {
        auto satDeg = solution_->examDegreeOfSaturation.at(unscheduledExam);
        auto deg = solution_->examData->examDegree.at(unscheduledExam);
        if (satDeg > maxSatDeg || (satDeg == maxSatDeg && deg > maxDeg)) {
            maxSatDeg = satDeg;
            maxDeg = deg;
            exam = unscheduledExam;
        }
    }
    auto result = solution_->examData->examSamePeriod.at(exam);
    result.insert(exam);

    for (auto &out: result)
        unscheduledExams.erase(out);

    return result;
}

std::set<int> ExamTTSolutionManipulator::removeExamDSatur(std::set<int> &unscheduledExams) {
    if (unscheduledExams.empty())
        return {};
    int exam = -1;
    auto minPeriods = solution_->examData->periodID.size();
    int maxSize = 0;
    for (auto &unscheduledExam: unscheduledExams) {
        int sum = 0;
        auto validity = solution_->examData->examPeriodsValidity.at(unscheduledExam);
        for (int periodIndex = 0; periodIndex < validity.size(); ++periodIndex) {
            if (solution_->periodExamCollisions.at(periodIndex).at(unscheduledExam) == 0)
                sum += validity.at(periodIndex);
        }
        auto size = solution_->examData->examSize.at(unscheduledExam);
        if(sum < minPeriods || (sum == minPeriods && size > maxSize)){
            exam = unscheduledExam;
            minPeriods = sum;
            maxSize = size;
        }
    }
    auto result = solution_->examData->examSamePeriod.at(exam);
    result.insert(exam);

    for (auto &out: result)
        unscheduledExams.erase(out);

    return result;
}

std::set<int> ExamTTSolutionManipulator::removeLargestExam(std::set<int> &unscheduledExams) {
    if (unscheduledExams.empty())
        return {};
    int limit = 0;
    for (int i = 0; i < solution_->examData->roomType.size(); ++i) {
        if (solution_->examData->roomType.at(i) == ExamTTData::RoomType::Normal)
            limit += solution_->examData->roomSize.at(i);
    }
    int exam = 0;
    int maxSize = solution_->examData->examSize.at(exam);
    for (auto &i: unscheduledExams) {
        if (solution_->examData->examSize.at(i) > maxSize) {
            exam = i;
            maxSize = solution_->examData->examSize.at(i);
        }
    }
    if (maxSize < limit)
        return {};

    auto result = solution_->examData->examSamePeriod.at(exam);
    result.insert(exam);

    for (auto &out: result)
        unscheduledExams.erase(out);

    return result;
}

int ExamTTSolutionManipulator::getRandomPeriod() {
    int maxPeriod = static_cast<int>(solution_->examData->periodDay.size() - 1);
    std::uniform_int_distribution<int> distPeriod(0, maxPeriod);
    return distPeriod(gen);
}

std::set<int> ExamTTSolutionManipulator::getValidPeriodsForExams(const std::set<int> &exams) {
    std::vector<std::vector<int>> vecs;
    for (auto &exam: exams)
        vecs.emplace_back(solution_->examData->examPeriodsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(vecs, [](const int &value) { return value == 1; });
}

std::set<int> ExamTTSolutionManipulator::getBestFittingRoomsForExamInSamePeriod(const int &exam) {
    if (exam < 0 || exam >= solution_->examPeriod.size())
        return {};
    auto period = solution_->examPeriod.at(exam);
    if (period < 0)
        return {};
    auto roomsAvailability = getPeriodRoomsAvailabilityWithout(period, {exam});
    //auto availableValidRooms = getAvailableValidRoomsForExamInPeriod(exam, period);
    auto availableValidRooms = getAvailableValidRoomsForExam(exam, roomsAvailability);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = solution_->examData->examSize.at(exam);
    auto numAndSumOfBinsReq = VectorUtils::getLeastNumberAndSumOfBinsRequired(examSize, roomSizes);
    if (numAndSumOfBinsReq.first == 0)
        return {};
    std::set<int> result;
    VectorUtils::binPackingSmallestAndLeastBins(examSize, numAndSumOfBinsReq.first, numAndSumOfBinsReq.second,
                                                roomSizes, result);
    return result;
}

std::set<int> ExamTTSolutionManipulator::getRandomRoomsForExamInSamePeriod(const int &exam) {
    if (exam < 0 || exam >= solution_->examPeriod.size())
        return {};
    auto period = solution_->examPeriod.at(exam);
    if (period < 0)
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExamInPeriod(exam, period);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = solution_->examData->examSize.at(exam);
    auto numberOfBinsRequired = VectorUtils::getLeastNumberAndSumOfBinsRequired(examSize, roomSizes);
    if (numberOfBinsRequired.first == 0)
        return {};
    std::vector<std::set<int>> combinations;
    std::set<int> currentBins;
    VectorUtils::binPackingLeastBinsAll(examSize, roomSizes, numberOfBinsRequired.first, combinations, currentBins, 0,
                                        0);
    if (combinations.empty())
        return {};
    std::uniform_int_distribution<std::size_t> distrRoom(0, combinations.size() - 1);
    auto roomCombinationIndex = distrRoom(gen);
    return combinations.at(roomCombinationIndex);
}

std::set<int> ExamTTSolutionManipulator::getRandomRoomsForExamInPeriod(const int &exam, const int &period) {
    if (exam < 0 || period < 0)
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExamInPeriod(exam, period);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = solution_->examData->examSize.at(exam);
    auto numberOfBinsRequired = VectorUtils::getLeastNumberAndSumOfBinsRequired(examSize, roomSizes);
    if (numberOfBinsRequired.first == 0)
        return {};
    std::vector<std::set<int>> combinations;
    std::set<int> currentBins;
    VectorUtils::binPackingLeastBinsAll(examSize, roomSizes, numberOfBinsRequired.first, combinations, currentBins, 0,
                                        0);
    if (combinations.empty())
        return {};
    std::uniform_int_distribution<std::size_t> distrRoom(0, combinations.size() - 1);
    auto roomCombinationIndex = distrRoom(gen);
    return combinations.at(roomCombinationIndex);
}

std::set<int>
ExamTTSolutionManipulator::getRandomRoomsForExam(const std::pair<int, int> &exam, const std::vector<int> &roomsAvailability) {
    if (exam.first < 0 || roomsAvailability.empty())
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExam(exam.first, roomsAvailability);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto numberOfBinsRequired = VectorUtils::getLeastNumberAndSumOfBinsRequired(exam.second, roomSizes);
    if (numberOfBinsRequired.first == 0)
        return {};
    std::vector<std::set<int>> combinations;
    VectorUtils::binPackingLeastBinsAll(exam.second, roomSizes, numberOfBinsRequired.first, combinations);
    if (combinations.empty())
        return {};
    std::uniform_int_distribution<std::size_t> distrRoom(0, combinations.size() - 1);
    auto roomCombinationIndex = distrRoom(gen);
    return combinations.at(roomCombinationIndex);
}

std::set<int>
ExamTTSolutionManipulator::getBestFittingRoomsForExam(const std::pair<int, int> &exam,
                                                      const std::vector<int> &roomsAvailability) {
    if (exam.first < 0 || roomsAvailability.empty())
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExam(exam.first, roomsAvailability);
    if (availableValidRooms.empty())
        return {};

    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = solution_->examData->examSize.at(exam.first);

    auto numAndSumOfBinsReq = VectorUtils::getLeastNumberAndSumOfBinsRequired(exam.second, roomSizes);
    if (numAndSumOfBinsReq.first == 0)
        return {};

    std::set<int> result;
    VectorUtils::binPackingSmallestAndLeastBins(examSize, numAndSumOfBinsReq.first, numAndSumOfBinsReq.second,
                                                roomSizes, result);
    return result;
}

void ExamTTSolutionManipulator::reassignRoomsToExamSamePeriod(const int &exam, std::set<int> &rooms) {
    auto period = solution_->examPeriod.at(exam);
    for (auto &room: solution_->examRooms.at(exam)) {
        solution_->periodRoomsAvailability.at(period).at(room) = 1;
    }
    solution_->examRooms.at(exam) = rooms;
    for (auto &room: solution_->examRooms.at(exam)) {
        solution_->periodRoomsAvailability.at(period).at(room) = 0;
    }
}

bool ExamTTSolutionManipulator::trySwitchUsedRooms(const int &periodFirst, const int &periodSecond) {
    auto roomsAvailabilityFirst = solution_->periodRoomsAvailability.at(periodFirst);
    auto roomsAvailabilitySecond = solution_->periodRoomsAvailability.at(periodSecond);
    for (int roomIndex = 0; roomIndex < roomsAvailabilityFirst.size(); ++roomIndex) {
        auto roomFirst = roomsAvailabilityFirst.at(roomIndex);
        auto roomSecond = roomsAvailabilitySecond.at(roomIndex);
        if (roomFirst == 0 && roomSecond == -1 || roomFirst == -1 && roomSecond == 0)
            return false;
        if (roomFirst == 0 && roomSecond == 1 || roomFirst == 1 && roomSecond == 0) {
            roomsAvailabilityFirst.at(roomIndex) = roomSecond;
            roomsAvailabilitySecond.at(roomIndex) = roomFirst;
        }
    }
    solution_->periodRoomsAvailability.at(periodFirst) = roomsAvailabilityFirst;
    solution_->periodRoomsAvailability.at(periodSecond) = roomsAvailabilitySecond;
    return true;
}

bool ExamTTSolutionManipulator::tryAssignBestFitRoomsForEachExamInOtherPeriod(const std::set<int> &examsFirst,
                                                                              const int &firstPeriod,
                                                                              const std::set<int> &examsSecond,
                                                                              const int &secondPeriod) {
    auto roomsAvailabilityFirst = getPeriodRoomsAvailabilityFreed(firstPeriod);
    auto roomsAvailabilitySecond = getPeriodRoomsAvailabilityFreed(secondPeriod);
    auto examsSizesFirst = getExamsIndexAndSize(examsFirst);
    auto examsSizesSecond = getExamsIndexAndSize(examsSecond);
    std::vector<std::pair<int, std::set<int>>> examRoomsFirst;
    for (auto &exam: examsSizesFirst) {
        auto rooms = getBestFittingRoomsForExam(exam, roomsAvailabilityFirst);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailabilityFirst.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailabilityFirst.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailabilityFirst.at(room) = 0;
        }
        examRoomsFirst.emplace_back(exam.first, rooms);
    }

    std::vector<std::pair<int, std::set<int>>> examRoomsSecond;
    for (auto &exam: examsSizesSecond) {
        auto rooms = getBestFittingRoomsForExam(exam, roomsAvailabilitySecond);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailabilitySecond.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailabilitySecond.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailabilitySecond.at(room) = 0;
        }
        examRoomsSecond.emplace_back(exam.first, rooms);
    }

    for (auto &pair: examRoomsFirst) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    solution_->periodRoomsAvailability.at(firstPeriod) = roomsAvailabilityFirst;

    for (auto &pair: examRoomsSecond) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    if (secondPeriod > -1)
        solution_->periodRoomsAvailability.at(secondPeriod) = roomsAvailabilitySecond;
    return true;
}

bool ExamTTSolutionManipulator::tryAssignRandomRoomsForEachExamInOtherPeriod(const std::set<int> &examsFirst,
                                                                             const int &firstPeriod,
                                                                             const std::set<int> &examsSecond,
                                                                             const int &secondPeriod) {
    auto roomsAvailabilityFirst = getPeriodRoomsAvailabilityWithout(firstPeriod, examsSecond);
    auto roomsAvailabilitySecond = getPeriodRoomsAvailabilityWithout(secondPeriod, examsFirst);
    auto examsSizesSecondToFirst = getExamsIndexAndSize(examsFirst);
    auto examsSizesFirstToSecond = getExamsIndexAndSize(examsSecond);
    std::vector<std::pair<int, std::set<int>>> examRoomsFirst;
    for (auto &exam: examsSizesSecondToFirst) {
        auto rooms = getRandomRoomsForExam(exam, roomsAvailabilityFirst);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailabilityFirst.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailabilityFirst.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailabilityFirst.at(room) = 0;
        }
        examRoomsFirst.emplace_back(exam.first, rooms);
    }

    std::vector<std::pair<int, std::set<int>>> examRoomsSecond;
    for (auto &exam: examsSizesFirstToSecond) {
        auto rooms = getRandomRoomsForExam(exam, roomsAvailabilitySecond);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailabilitySecond.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailabilitySecond.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailabilitySecond.at(room) = 0;
        }
        examRoomsSecond.emplace_back(exam.first, rooms);
    }

    for (auto &pair: examRoomsFirst) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    solution_->periodRoomsAvailability.at(firstPeriod) = roomsAvailabilityFirst;

    for (auto &pair: examRoomsSecond) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    if (secondPeriod > -1)
        solution_->periodRoomsAvailability.at(secondPeriod) = roomsAvailabilitySecond;
    return true;
}

bool
ExamTTSolutionManipulator::tryAssignBestFittingRoomsForEachExamInPeriodWithout(const std::set<int> &exams, const int &period,
                                                                               const std::set<int> &without) {
    auto roomsAvailability = getPeriodRoomsAvailabilityWithout(period, without);
    auto examsSizes = getExamsIndexAndSize(exams);
    std::vector<std::pair<int, std::set<int>>> examRooms;
    for (auto &exam: examsSizes) {
        auto rooms = getBestFittingRoomsForExam(exam, roomsAvailability);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailability.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailability.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailability.at(room) = 0;
        }
        examRooms.emplace_back(exam.first, rooms);
    }
    for (auto &pair: examRooms) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    solution_->periodRoomsAvailability.at(period) = roomsAvailability;
    return true;
}

bool ExamTTSolutionManipulator::tryAssignBestFittingRoomsForEachExamInPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period < 0)
        return false;
    auto examsAll = solution_->periodExams.at(period);
    examsAll.insert(exams.begin(), exams.end());
    return tryAssignBestFitRoomsForEachExamInOtherPeriod(examsAll, period, {}, -1);
}

bool ExamTTSolutionManipulator::tryAssignRandomRoomsForEachExamInPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period < 0)
        return false;
    return tryAssignRandomRoomsForEachExamInOtherPeriod(exams, period, {}, -1);
}

void ExamTTSolutionManipulator::moveExamToPeriod(const int &exam, const int &period) {
    if (exam == -1)
        return;
    auto oldPeriod = solution_->examPeriod.at(exam);
    if (oldPeriod == period)
        return;
    for (auto &collisionExam: solution_->examData->examCollisionExams.at(exam)) {
        if (oldPeriod != -1)
            if (--solution_->periodExamCollisions.at(oldPeriod).at(collisionExam) == 0)
                --solution_->examDegreeOfSaturation.at(collisionExam);
        if (period != -1)
            if (++solution_->periodExamCollisions.at(period).at(collisionExam) == 1)
                ++solution_->examDegreeOfSaturation.at(collisionExam);
    }
    if (oldPeriod != -1)
        solution_->periodExams.at(oldPeriod).erase(exam);
    if (period != -1)
        solution_->periodExams.at(period).insert(exam);
    solution_->examPeriod.at(exam) = period;
}

void ExamTTSolutionManipulator::moveExamsToPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty())
        return;
    for (auto &exam: exams)
        moveExamToPeriod(exam, period);
}

void ExamTTSolutionManipulator::kempeChain(std::set<int> &displacedFirst, std::set<int> &displacedSecond,
                                           std::set<int> &examsFirst, std::set<int> &examsSecond) {
    std::set<int> extracted = extractCollisionsAndConnected(displacedFirst, examsSecond);
    if (!extracted.empty())
        displacedSecond.insert(extracted.begin(), extracted.end());

    extracted = extractCollisionsAndConnected(displacedSecond, examsFirst);
    if (extracted.empty())
        return;
    displacedFirst.insert(extracted.begin(), extracted.end());

    while (true) {
        extracted = extractCollisionsAndConnected(extracted, examsSecond);
        if (extracted.empty())
            return;
        displacedSecond.insert(extracted.begin(), extracted.end());

        extracted = extractCollisionsAndConnected(extracted, examsFirst);
        if (extracted.empty())
            return;
        displacedFirst.insert(extracted.begin(), extracted.end());
    }
}

bool ExamTTSolutionManipulator::hasAnyExamCollisionWithPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period == -1)
        return false;
    return std::any_of(exams.begin(), exams.end(),
                       [&](const int &exam) { return solution_->periodExamCollisions.at(period).at(exam) > 0; });
}

bool ExamTTSolutionManipulator::isAnyExamInvalidInPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period == -1)
        return false;
    return std::any_of(exams.begin(), exams.end(),
                       [&](const int &exam) { return solution_->examData->examPeriodsValidity.at(exam).at(period) == 0; });
}

std::set<int> ExamTTSolutionManipulator::getExamsInPeriodWithout(const std::set<int> &without) {
    auto period = solution_->examPeriod.at(*(without.begin()));
    auto exams = solution_->periodExams.at(period);
    for (auto &exam: without)
        exams.erase(exam);
    return exams;
}

std::set<int> ExamTTSolutionManipulator::getExamsInPeriod(const int &period) const {
    if (period == -1)
        return {};
    return solution_->periodExams.at(period);
}

std::vector<int>
ExamTTSolutionManipulator::getPeriodRoomsAvailabilityWithout(const int &period, const std::set<int> &without) {
    if (period == -1)
        return {};
    auto result = solution_->periodRoomsAvailability.at(period);
    for (auto &exam: without) {
        for (auto &room: solution_->examRooms.at(exam)) {
            result.at(room) = 1;
        }
    }
    return result;
}

std::vector<int> ExamTTSolutionManipulator::getPeriodRoomsAvailabilityFreed(const int &period) {
    if (period == -1)
        return {};
    auto result = solution_->periodRoomsAvailability.at(period);
    for (auto &room: result) {
        if (room == 0)
            room = 1;
    }
    return result;
}

std::set<int> ExamTTSolutionManipulator::getAllExams() const {
    std::set<int> exams;
    for (int i = 0; i < solution_->examData->examID.size(); ++i)
        exams.insert(i);
    return exams;
}

int ExamTTSolutionManipulator::getNumberOfPeriods() {
    return static_cast<int>(solution_->examData->periodID.size());
}

std::set<int> ExamTTSolutionManipulator::extractCollisionsAndConnected(const std::set<int> &inserts, std::set<int> &exams) {
    std::set<int> result;
    // inserts are supposed to be inserted into the period with exams
    for (const auto &ins: inserts) {
        // All exams in the period that have collision with the inserts are extracted into a separate set to check only their connected exams
        std::set<int> extractedCollisions;
        std::copy_if(exams.begin(), exams.end(), std::inserter(extractedCollisions, extractedCollisions.begin()),
                     [&](const int &value) { return solution_->examData->examsCollisions.at(ins).at(value) > 0; });
        // The exams that have to be in the same period as the extracted are also extracted and inserted in the return set
        for (auto &ex: extractedCollisions) {
            auto same = solution_->examData->examSamePeriod.at(ex);
            std::remove_copy_if(exams.begin(), exams.end(), std::inserter(result, result.begin()),
                                [&](const int &value) {
                                    return same.end() == std::find(same.begin(), same.end(), value);
                                });
        }
        // the extracted in the separate set are now added to the return set
        result.insert(extractedCollisions.begin(), extractedCollisions.end());
    }
    // remove all copied values from source container
    for (auto &val: result)
        exams.erase(val);
    return result;
}

std::set<int> ExamTTSolutionManipulator::getAvailableValidRoomsForExamInPeriod(const int &exam, const int &period) {
    std::vector<std::vector<int>> tmp;
    tmp.push_back(solution_->periodRoomsAvailability.at(period));
    tmp.push_back(solution_->examData->examRoomsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(tmp, [](const int &value) { return value == 1; });
}

std::set<int>
ExamTTSolutionManipulator::getAvailableValidRoomsForExam(const int &exam, const std::vector<int> &roomsAvailability) {
    std::vector<std::vector<int>> tmp;
    tmp.push_back(roomsAvailability);
    tmp.push_back(solution_->examData->examRoomsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(tmp, [](const int &value) { return value == 1; });
}

std::vector<std::pair<int, int>> ExamTTSolutionManipulator::getRoomsIndexAndSize(const std::set<int> &rooms) {
    std::vector<std::pair<int, int>> roomSizes;
    for (auto &room: rooms)
        roomSizes.emplace_back(room, solution_->examData->roomSize.at(room));
    std::sort(roomSizes.begin(), roomSizes.end(),
              [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });
    return roomSizes;
}

std::vector<std::pair<int, int>> ExamTTSolutionManipulator::getExamsIndexAndSize(const std::set<int> &exams) {
    if (exams.empty())
        return {};
    std::vector<std::pair<int, int>> examSizes;
    for (auto &exam: exams)
        examSizes.emplace_back(exam, solution_->examData->examSize.at(exam));
    std::sort(examSizes.begin(), examSizes.end(),
              [](std::pair<int, int> a, std::pair<int, int> b) { return a.second > b.second; });
    return examSizes;
}

int ExamTTSolutionManipulator::getPreviousPeriodSameDay(const int &period) {
    if (period < 1 || solution_->examData->periodDay.at(period - 1) != solution_->examData->periodDay.at(period))
        return -1;
    return period - 1;
}

int ExamTTSolutionManipulator::getNextPeriodSameDay(const int &period) {
    if (period + 1 >= solution_->examData->periodDay.size() ||
            solution_->examData->periodDay.at(period + 1) != solution_->examData->periodDay.at(period))
        return -1;
    return period + 1;
}


//
// Created by hansk on 21.05.2024.
//

#include <iostream>
#include <set>
#include <utility>
#include "ExamTTSolutionManipulator.h"
#include "vectorUtils/VectorUtils.h"
#include "vectorUtils/PeriodChange.h"
#include "screenOutput.h"


void ExamTTSolutionManipulator::setSolution(const std::shared_ptr<ExamTTSolution> &solution) {
    this->solution_ = std::make_shared<ExamTTSolution>(*solution);
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
        if (sum < minPeriods || (sum == minPeriods && size > maxSize)) {
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
        if (solution_->examData->roomType.at(i) == RoomType::Normal)
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

std::set<int>
ExamTTSolutionManipulator::getRandomRoomsForExam(const int exam, const std::vector<int> &roomsAvailability,
                                                 const int randomSampleSize) {
    if (exam < 0 || roomsAvailability.empty())
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExam(exam, roomsAvailability);
    if (availableValidRooms.empty())
        return {};
    auto combinations = VectorUtils::getSubsets(availableValidRooms,
                                                solution_->examData->examsPossibleRoomCombinations.at(exam),
                                                randomSampleSize);
    if (!combinations.has_value())
        return {};
    if (combinations->size() == 1)
        return combinations->front();
    std::uniform_int_distribution<std::size_t> distrRoom(0, combinations->size() - 1);
    auto roomCombinationIndex = distrRoom(gen);
    return combinations->at(roomCombinationIndex);
}

bool ExamTTSolutionManipulator::tryReassignRoomsToExamSamePeriod(int randomSampleSize, const int exam) {
    auto period = solution_->examPeriod.at(exam);
    return tryAssignRandomRoomsForEachExamInOtherPeriod(randomSampleSize, PeriodChange(period, {exam}));
}

bool ExamTTSolutionManipulator::trySwitchUsedRooms(const int periodFirst, const int periodSecond) {
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

bool ExamTTSolutionManipulator::tryAssignRandomRoomsForEachExamInOtherPeriod(const int randomSampleSize,
                                                                             const PeriodChange &first,
                                                                             const PeriodChange &second) {
    auto roomsAvailabilityFirst = getPeriodRoomsAvailabilityWithout(first.period, first.moveOut);
    auto roomsAvailabilitySecond = getPeriodRoomsAvailabilityWithout(second.period, second.moveOut);

    std::vector<std::pair<int, std::set<int>>> examRoomsFirst;
    for (auto &exam: first.moveIn) {
        auto rooms = getRandomRoomsForExam(exam, roomsAvailabilityFirst, randomSampleSize);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailabilityFirst.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailabilityFirst.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailabilityFirst.at(room) = 0;
        }
        examRoomsFirst.emplace_back(exam, rooms);
    }

    std::vector<std::pair<int, std::set<int>>> examRoomsSecond;
    for (auto &exam: second.moveIn) {
        auto rooms = getRandomRoomsForExam(exam, roomsAvailabilitySecond, randomSampleSize);
        if (rooms.empty())
            return false;
        for (auto &room: rooms) {
            if (roomsAvailabilitySecond.at(room) == -1)
                throw std::runtime_error("tried assigning an invalid room");
            if (roomsAvailabilitySecond.at(room) == 0)
                throw std::runtime_error("tried assigning an unavailable room");
            roomsAvailabilitySecond.at(room) = 0;
        }
        examRoomsSecond.emplace_back(exam, rooms);
    }

    for (auto &pair: examRoomsFirst) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    solution_->periodRoomsAvailability.at(first.period) = roomsAvailabilityFirst;

    for (auto &pair: examRoomsSecond) {
        solution_->examRooms.at(pair.first) = pair.second;
    }
    if (second.period > -1)
        solution_->periodRoomsAvailability.at(second.period) = roomsAvailabilitySecond;
    return true;
}

void ExamTTSolutionManipulator::moveExamToPeriod(const int &exam, const int &period) {
    if (exam == -1)
        return;
    if (exam >= solution_->examPeriod.size())
        throw std::invalid_argument("exam index is out of range of examPeriod");
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
                       [&](const int &exam) {
                           return solution_->examData->examPeriodsValidity.at(exam).at(period) == 0;
                       });
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

std::set<int> ExamTTSolutionManipulator::getAllExams() const {
    std::set<int> exams;
    for (int i = 0; i < solution_->examData->examID.size(); ++i)
        exams.insert(i);
    return exams;
}

std::set<int>
ExamTTSolutionManipulator::extractCollisionsAndConnected(const std::set<int> &inserts, std::set<int> &exams) {
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

std::set<int>
ExamTTSolutionManipulator::getAvailableValidRoomsForExam(const int &exam, const std::vector<int> &roomsAvailability) {
    std::vector<std::vector<int>> tmp;
    tmp.push_back(roomsAvailability);
    tmp.push_back(solution_->examData->examRoomsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(tmp, [](const int &value) { return value == 1; });
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


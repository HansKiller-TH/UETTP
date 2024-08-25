//
// Created by hansk on 21.05.2024.
//

#include <iostream>
#include <set>
#include <utility>
#include "ExamDataManipulator.h"
#include "vectorUtils/VectorUtils.h"
#include "screenOutput.h"


void ExamDataManipulator::setData(std::shared_ptr<ExamTTData> examDataCurrent) {
    this->examData_ = std::move(examDataCurrent);
}

std::shared_ptr<ExamTTData> ExamDataManipulator::getData() {
    return this->examData_;
}

int ExamDataManipulator::getRandomExam() {
    int max = static_cast<int>(examData_->examID.size() - 1);
    std::uniform_int_distribution<> distr(0, max);
    return distr(gen);
}

std::set<int> ExamDataManipulator::getRandomExamWithConnected() {
    auto rand = getRandomExam();
    std::set<int> exams = examData_->examSamePeriod.at(rand);
    exams.insert(rand);
    return exams;
}

std::pair<std::set<int>, int> ExamDataManipulator::getRandomExamWithConnectedWithPeriod() {
    auto exams = getRandomExamWithConnected();
    auto period = examData_->examPeriod.at(*(exams.begin()));
    return {exams, period};
}

std::set<int> ExamDataManipulator::removeExamWithLargestDegreeOfSaturation(std::set<int> &unscheduledExams) {
    if (unscheduledExams.empty())
        return {};
    int exam = -1;
    int maxDeg = -1;
    int maxSatDeg = -1;
    for (auto &unscheduledExam: unscheduledExams) {
        auto satDeg = examData_->examDegreeOfSaturation.at(unscheduledExam);
        auto deg = examData_->examDegree.at(unscheduledExam);
        if (satDeg > maxSatDeg || (satDeg == maxSatDeg && deg > maxDeg)) {
            maxSatDeg = satDeg;
            maxDeg = deg;
            exam = unscheduledExam;
        }
    }
    auto result = examData_->examSamePeriod.at(exam);
    result.insert(exam);

    for (auto &out: result)
        unscheduledExams.erase(out);

    return result;
}

std::set<int> ExamDataManipulator::removeExamDSatur(std::set<int> &unscheduledExams) {
    if (unscheduledExams.empty())
        return {};
    int exam = -1;
    auto minPeriods = examData_->periodID.size();
    int maxSize = 0;
    for (auto &unscheduledExam: unscheduledExams) {
        int sum = 0;
        auto validity = examData_->examPeriodsValidity.at(unscheduledExam);
        for (int periodIndex = 0; periodIndex < validity.size(); ++periodIndex) {
            if (examData_->periodExamCollisions.at(periodIndex).at(unscheduledExam) == 0)
                sum += validity.at(periodIndex);
        }
        auto size = examData_->examSize.at(unscheduledExam);
        if(sum < minPeriods || (sum == minPeriods && size > maxSize)){
            exam = unscheduledExam;
            minPeriods = sum;
            maxSize = size;
        }
    }
    auto result = examData_->examSamePeriod.at(exam);
    result.insert(exam);

    for (auto &out: result)
        unscheduledExams.erase(out);

    return result;
}

std::set<int> ExamDataManipulator::removeLargestExam(std::set<int> &unscheduledExams) {
    if (unscheduledExams.empty())
        return {};
    int limit = 0;
    for (int i = 0; i < examData_->roomType.size(); ++i) {
        if (examData_->roomType.at(i) == ExamTTData::RoomType::Normal)
            limit += examData_->roomSize.at(i);
    }
    int exam = 0;
    int maxSize = examData_->examSize.at(exam);
    for (auto &i: unscheduledExams) {
        if (examData_->examSize.at(i) > maxSize) {
            exam = i;
            maxSize = examData_->examSize.at(i);
        }
    }
    if (maxSize < limit)
        return {};

    auto result = examData_->examSamePeriod.at(exam);
    result.insert(exam);

    for (auto &out: result)
        unscheduledExams.erase(out);

    return result;
}

int ExamDataManipulator::getRandomPeriod() {
    int maxPeriod = static_cast<int>(examData_->periodDay.size() - 1);
    std::uniform_int_distribution<int> distPeriod(0, maxPeriod);
    return distPeriod(gen);
}

std::set<int> ExamDataManipulator::getValidPeriodsForExams(const std::set<int> &exams) {
    std::vector<std::vector<int>> vecs;
    for (auto &exam: exams)
        vecs.emplace_back(examData_->examPeriodsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(vecs, [](const int &value) { return value == 1; });
}

std::set<int> ExamDataManipulator::getBestFittingRoomsForExamInSamePeriod(const int &exam) {
    if (exam < 0 || exam >= examData_->examPeriod.size())
        return {};
    auto period = examData_->examPeriod.at(exam);
    if (period < 0)
        return {};
    auto roomsAvailability = getPeriodRoomsAvailabilityWithout(period, {exam});
    //auto availableValidRooms = getAvailableValidRoomsForExamInPeriod(exam, period);
    auto availableValidRooms = getAvailableValidRoomsForExam(exam, roomsAvailability);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = examData_->examSize.at(exam);
    auto numAndSumOfBinsReq = VectorUtils::getLeastNumberAndSumOfBinsRequired(examSize, roomSizes);
    if (numAndSumOfBinsReq.first == 0)
        return {};
    std::set<int> result;
    VectorUtils::binPackingSmallestAndLeastBins(examSize, numAndSumOfBinsReq.first, numAndSumOfBinsReq.second,
                                                roomSizes, result);
    return result;
}

std::set<int> ExamDataManipulator::getRandomRoomsForExamInSamePeriod(const int &exam) {
    if (exam < 0 || exam >= examData_->examPeriod.size())
        return {};
    auto period = examData_->examPeriod.at(exam);
    if (period < 0)
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExamInPeriod(exam, period);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = examData_->examSize.at(exam);
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

std::set<int> ExamDataManipulator::getRandomRoomsForExamInPeriod(const int &exam, const int &period) {
    if (exam < 0 || period < 0)
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExamInPeriod(exam, period);
    if (availableValidRooms.empty())
        return {};
    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = examData_->examSize.at(exam);
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
ExamDataManipulator::getRandomRoomsForExam(const std::pair<int, int> &exam, const std::vector<int> &roomsAvailability) {
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
ExamDataManipulator::getBestFittingRoomsForExam(const std::pair<int, int> &exam,
                                                const std::vector<int> &roomsAvailability) {
    if (exam.first < 0 || roomsAvailability.empty())
        return {};
    auto availableValidRooms = getAvailableValidRoomsForExam(exam.first, roomsAvailability);
    if (availableValidRooms.empty())
        return {};

    auto roomSizes = getRoomsIndexAndSize(availableValidRooms);
    auto examSize = examData_->examSize.at(exam.first);

    auto numAndSumOfBinsReq = VectorUtils::getLeastNumberAndSumOfBinsRequired(exam.second, roomSizes);
    if (numAndSumOfBinsReq.first == 0)
        return {};

    std::set<int> result;
    VectorUtils::binPackingSmallestAndLeastBins(examSize, numAndSumOfBinsReq.first, numAndSumOfBinsReq.second,
                                                roomSizes, result);
    return result;
}

void ExamDataManipulator::reassignRoomsToExamSamePeriod(const int &exam, std::set<int> &rooms) {
    auto period = examData_->examPeriod.at(exam);
    for (auto &room: examData_->examRooms.at(exam)) {
        examData_->periodRoomsAvailability.at(period).at(room) = 1;
    }
    examData_->examRooms.at(exam) = rooms;
    for (auto &room: examData_->examRooms.at(exam)) {
        examData_->periodRoomsAvailability.at(period).at(room) = 0;
    }
}

bool ExamDataManipulator::trySwitchUsedRooms(const int &periodFirst, const int &periodSecond) {
    auto roomsAvailabilityFirst = examData_->periodRoomsAvailability.at(periodFirst);
    auto roomsAvailabilitySecond = examData_->periodRoomsAvailability.at(periodSecond);
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
    examData_->periodRoomsAvailability.at(periodFirst) = roomsAvailabilityFirst;
    examData_->periodRoomsAvailability.at(periodSecond) = roomsAvailabilitySecond;
    return true;
}

bool ExamDataManipulator::tryAssignBestFitRoomsForEachExamInOtherPeriod(const std::set<int> &examsFirst,
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
        examData_->examRooms.at(pair.first) = pair.second;
    }
    examData_->periodRoomsAvailability.at(firstPeriod) = roomsAvailabilityFirst;

    for (auto &pair: examRoomsSecond) {
        examData_->examRooms.at(pair.first) = pair.second;
    }
    if (secondPeriod > -1)
        examData_->periodRoomsAvailability.at(secondPeriod) = roomsAvailabilitySecond;
    return true;
}

bool ExamDataManipulator::tryAssignRandomRoomsForEachExamInOtherPeriod(const std::set<int> &examsFirst,
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
        examData_->examRooms.at(pair.first) = pair.second;
    }
    examData_->periodRoomsAvailability.at(firstPeriod) = roomsAvailabilityFirst;

    for (auto &pair: examRoomsSecond) {
        examData_->examRooms.at(pair.first) = pair.second;
    }
    if (secondPeriod > -1)
        examData_->periodRoomsAvailability.at(secondPeriod) = roomsAvailabilitySecond;
    return true;
}

bool
ExamDataManipulator::tryAssignBestFittingRoomsForEachExamInPeriodWithout(const std::set<int> &exams, const int &period,
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
        examData_->examRooms.at(pair.first) = pair.second;
    }
    examData_->periodRoomsAvailability.at(period) = roomsAvailability;
    return true;
}

bool ExamDataManipulator::tryAssignBestFittingRoomsForEachExamInPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period < 0)
        return false;
    auto examsAll = examData_->periodExams.at(period);
    examsAll.insert(exams.begin(), exams.end());
    return tryAssignBestFitRoomsForEachExamInOtherPeriod(examsAll, period, {}, -1);
}

bool ExamDataManipulator::tryAssignRandomRoomsForEachExamInPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period < 0)
        return false;
    return tryAssignRandomRoomsForEachExamInOtherPeriod(exams, period, {}, -1);
}

void ExamDataManipulator::moveExamToPeriod(const int &exam, const int &period) {
    if (exam == -1)
        return;
    auto oldPeriod = examData_->examPeriod.at(exam);
    if (oldPeriod == period)
        return;
    for (auto &collisionExam: examData_->examCollisionExams.at(exam)) {
        if (oldPeriod != -1)
            if (--examData_->periodExamCollisions.at(oldPeriod).at(collisionExam) == 0)
                --examData_->examDegreeOfSaturation.at(collisionExam);
        if (period != -1)
            if (++examData_->periodExamCollisions.at(period).at(collisionExam) == 1)
                ++examData_->examDegreeOfSaturation.at(collisionExam);
    }
    if (oldPeriod != -1)
        examData_->periodExams.at(oldPeriod).erase(exam);
    if (period != -1)
        examData_->periodExams.at(period).insert(exam);
    examData_->examPeriod.at(exam) = period;
}

void ExamDataManipulator::moveExamsToPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty())
        return;
    for (auto &exam: exams)
        moveExamToPeriod(exam, period);
}

void ExamDataManipulator::kempeChain(std::set<int> &displacedFirst, std::set<int> &displacedSecond,
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

bool ExamDataManipulator::hasAnyExamCollisionWithPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period == -1)
        return false;
    return std::any_of(exams.begin(), exams.end(),
                       [&](const int &exam) { return examData_->periodExamCollisions.at(period).at(exam) > 0; });
}

bool ExamDataManipulator::isAnyExamInvalidInPeriod(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period == -1)
        return false;
    return std::any_of(exams.begin(), exams.end(),
                       [&](const int &exam) { return examData_->examPeriodsValidity.at(exam).at(period) == 0; });
}

std::set<int> ExamDataManipulator::getExamsInPeriodWithout(const std::set<int> &without) {
    auto period = examData_->examPeriod.at(*(without.begin()));
    auto exams = examData_->periodExams.at(period);
    for (auto &exam: without)
        exams.erase(exam);
    return exams;
}

std::set<int> ExamDataManipulator::getExamsInPeriod(const int &period) const {
    if (period == -1)
        return {};
    return examData_->periodExams.at(period);
}

std::vector<int>
ExamDataManipulator::getPeriodRoomsAvailabilityWithout(const int &period, const std::set<int> &without) {
    if (period == -1)
        return {};
    auto result = examData_->periodRoomsAvailability.at(period);
    for (auto &exam: without) {
        for (auto &room: examData_->examRooms.at(exam)) {
            result.at(room) = 1;
        }
    }
    return result;
}

std::vector<int> ExamDataManipulator::getPeriodRoomsAvailabilityFreed(const int &period) {
    if (period == -1)
        return {};
    auto result = examData_->periodRoomsAvailability.at(period);
    for (auto &room: result) {
        if (room == 0)
            room = 1;
    }
    return result;
}

std::set<int> ExamDataManipulator::getAllExams() const {
    std::set<int> exams;
    for (int i = 0; i < examData_->examID.size(); ++i)
        exams.insert(i);
    return exams;
}

int ExamDataManipulator::getNumberOfPeriods() {
    return static_cast<int>(examData_->periodID.size());
}

std::set<int> ExamDataManipulator::extractCollisionsAndConnected(const std::set<int> &inserts, std::set<int> &exams) {
    std::set<int> result;
    // inserts are supposed to be inserted into the period with exams
    for (const auto &ins: inserts) {
        // All exams in the period that have collision with the inserts are extracted into a separate set to check only their connected exams
        std::set<int> extractedCollisions;
        std::copy_if(exams.begin(), exams.end(), std::inserter(extractedCollisions, extractedCollisions.begin()),
                     [&](const int &value) { return examData_->examsCollisions.at(ins).at(value) > 0; });
        // The exams that have to be in the same period as the extracted are also extracted and inserted in the return set
        for (auto &ex: extractedCollisions) {
            auto same = examData_->examSamePeriod.at(ex);
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

std::set<int> ExamDataManipulator::getAvailableValidRoomsForExamInPeriod(const int &exam, const int &period) {
    std::vector<std::vector<int>> tmp;
    tmp.push_back(examData_->periodRoomsAvailability.at(period));
    tmp.push_back(examData_->examRoomsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(tmp, [](const int &value) { return value == 1; });
}

std::set<int>
ExamDataManipulator::getAvailableValidRoomsForExam(const int &exam, const std::vector<int> &roomsAvailability) {
    std::vector<std::vector<int>> tmp;
    tmp.push_back(roomsAvailability);
    tmp.push_back(examData_->examRoomsValidity.at(exam));
    return VectorUtils::getIndexesWherePredicateAllOf(tmp, [](const int &value) { return value == 1; });
}

std::vector<std::pair<int, int>> ExamDataManipulator::getRoomsIndexAndSize(const std::set<int> &rooms) {
    std::vector<std::pair<int, int>> roomSizes;
    for (auto &room: rooms)
        roomSizes.emplace_back(room, examData_->roomSize.at(room));
    std::sort(roomSizes.begin(), roomSizes.end(),
              [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });
    return roomSizes;
}

std::vector<std::pair<int, int>> ExamDataManipulator::getExamsIndexAndSize(const std::set<int> &exams) {
    if (exams.empty())
        return {};
    std::vector<std::pair<int, int>> examSizes;
    for (auto &exam: exams)
        examSizes.emplace_back(exam, examData_->examSize.at(exam));
    std::sort(examSizes.begin(), examSizes.end(),
              [](std::pair<int, int> a, std::pair<int, int> b) { return a.second > b.second; });
    return examSizes;
}

int ExamDataManipulator::getPreviousPeriodSameDay(const int &period) {
    if (period < 1 || examData_->periodDay.at(period - 1) != examData_->periodDay.at(period))
        return -1;
    return period - 1;
}

int ExamDataManipulator::getNextPeriodSameDay(const int &period) {
    if (period + 1 >= examData_->periodDay.size() ||
        examData_->periodDay.at(period + 1) != examData_->periodDay.at(period))
        return -1;
    return period + 1;
}


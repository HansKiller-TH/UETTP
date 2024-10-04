//
// Created by hansk on 04.06.2024.
//

#include <stdexcept>
#include "ExamTTDataBuilder.h"
#include "vectorUtils/VectorUtils.h"

ExamTTDataBuilder ExamTTDataBuilder::processPeriods(const std::vector<std::vector<std::string>> &periods) {
    for (auto &period: periods) {
        if (period.size() < 5)
            throw std::invalid_argument("Each Element of periods has to have 5 elements.");
        tmp.periodID->push_back(std::stoi(period.at(0)));
        tmp.periodDay->push_back(std::stoi(period.at(1)));
        tmp.periodSlot->push_back(std::stoi(period.at(2)));
        tmp.periodWeek->push_back(std::stoi(period.at(3)));
        tmp.periodDate->push_back(period.at(4));

    }
    return *this;
}

ExamTTDataBuilder ExamTTDataBuilder::processRooms(const std::vector<std::vector<std::string>> &rooms) {
    for (auto &room: rooms) {
        if (room.size() < 4)
            throw std::invalid_argument("Each Element of rooms has to have 4 elements.");
        tmp.roomID->push_back(std::stoi(room.at(0)));
        tmp.roomName->push_back(room.at(1));
        tmp.roomSize->push_back(std::stoi(room.at(2)));
        tmp.roomType->push_back(stringToRoomType(room.at(3)));
    }
    return *this;
}

ExamTTDataBuilder ExamTTDataBuilder::processRoomsValidPeriods(const std::vector<std::vector<std::string>> &roomsPeriods) {
    // add all roomsPeriods with initially all periods invalid with value 0
    tmp.roomPeriodsValidity->resize(tmp.roomID->size(), std::vector<int>(tmp.periodID->size(), 0));
    // add all periods with initially all roomsPeriods invalid with value -1
    // periodRoomsAvailability.resize(periodID.size(), std::vector<int>(roomID.size(), -1));
    for (auto &row: roomsPeriods) {
        auto room = VectorUtils::indexForValue(tmp.roomID.value(), row.at(0));
        if (room == -1)
            continue;
        auto period = VectorUtils::indexForValue(tmp.periodID.value(), row.at(1));
        if (period == -1)
            continue;
        //validate this period for the room
        tmp.roomPeriodsValidity->at(room).at(period) = 1;
        //mark the room available (and valid) in this period // necessary
        // periodRoomsAvailability.at(period).at(room) = 1;
    }
    return *this;
}

ExamTTDataBuilder ExamTTDataBuilder::processExams(const std::vector<std::vector<std::string>> &exams) {
    for (auto &exam: exams) {
        if (exam.size() < 3)
            throw std::invalid_argument("Each element of exams needs to have 3 elements");
        tmp.examID->push_back(std::stoi(exam.at(0)));
        tmp.examName->push_back(exam.at(1));
        tmp.examSize->push_back(std::stoi(exam.at(2))); //necessary but could easily be summed up from enrollment
    }
    return *this;
}

// necessary
ExamTTDataBuilder ExamTTDataBuilder::processExamValidPeriods(const std::vector<std::vector<std::string>> &examsPeriods) {
    // add all examsPeriods with initially all periods invalid
    tmp.examPeriodsValidity->resize(tmp.examID->size(), std::vector<int>(tmp.periodID->size(), 0));
    for (auto &row: examsPeriods) {
        auto exam = VectorUtils::indexForValue(tmp.examID.value(), row.at(0));
        if (exam == -1)
            continue;
        auto period = VectorUtils::indexForValue(tmp.periodID.value(), row.at(1));
        if (period == -1)
            continue;
        tmp.examPeriodsValidity->at(exam).at(period) = 1; //validate this row for the exam
    }
    return *this;
}

// necessary
ExamTTDataBuilder ExamTTDataBuilder::processExamsValidRooms(const std::vector<std::vector<std::string>> &examsRooms) {
    // add all examsRooms with initially all rooms invalid
    tmp.examRoomsValidity->resize(tmp.examID->size(), std::vector<int>(tmp.roomID->size(), 0));
    for (auto &row: examsRooms) {
        auto exam = VectorUtils::indexForValue(tmp.examID.value(), row.at(0));
        if (exam == -1)
            continue;
        auto room = VectorUtils::indexForValue(tmp.roomID.value(), row.at(1));
        if (room == -1)
            continue;
        tmp.examRoomsValidity->at(exam).at(room) = 1; //validate this row for the exam
    }
    return *this;
}

// necessary
ExamTTDataBuilder ExamTTDataBuilder::processStudentsExams(const std::vector<std::vector<std::string>> &studentsExams) {
    std::string prev = "-1";
    for (auto &row: studentsExams) {
        if (row.at(0) == "-1")
            continue;
        if (row.at(0) != prev) {
            prev = row.at(0);
            tmp.enrollment->emplace_back(); //add a student without enrolled row
        }
        auto exam = VectorUtils::indexForValue(tmp.examID.value(), row.at(1));
        if (exam == -1)
            continue;
        tmp.enrollment->back().insert(exam);
    }
    return *this;
}

// necessary
ExamTTDataBuilder ExamTTDataBuilder::processSamePeriods(const std::vector<std::vector<std::string>> &exams) {
    // We use this to check if we encountered a new set of connected exams
    std::string prev;
    // In this we gather all sets of connected exams before processing them
    std::vector<std::set<std::string>> gatheredSamePeriod;
    // Set up the vector
    tmp.examSamePeriod->resize(tmp.examID->size());

    // Gather all sets of connected exams
    for (auto &row: exams) {
        // if we come to a new set of connected exams
        if (row.at(0) != prev) {
            prev = row.at(0);
            // add a new set
            gatheredSamePeriod.emplace_back();
        }
        // We insert the exam ID into the set of connected exams
        gatheredSamePeriod.back().insert(row.at(1));
    }

    // process all sets of connected exams
    for (auto &same: gatheredSamePeriod) {
        for (auto &exam: same) {
            for (auto &otherExam: same) {
                if (exam == otherExam)
                    continue;
                int examIndex = VectorUtils::indexForValue(tmp.examID.value(), exam);
                if (examIndex == -1)
                    continue;
                int otherExamIndex = VectorUtils::indexForValue(tmp.examID.value(), otherExam);
                if (otherExamIndex == -1)
                    continue;
                tmp.examSamePeriod->at(examIndex).insert(otherExamIndex);
            }
        }
    }
    return *this;
}

ExamTTDataBuilder ExamTTDataBuilder::createCollisionsFromEnrollment() {
    const auto NUMBER_OF_EXAMS = tmp.examID->size();
    // set up data structures
    tmp.examsCollisions->resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < tmp.examsCollisions->size(); ++i)
        tmp.examsCollisions->at(i).at(i) = -1;
    tmp.examsCollisionsLimit->resize(15,
                                std::vector<std::vector<int>>(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0)));
    for (auto &matrix: tmp.examsCollisionsLimit.value())
        for (int i = 0; i < matrix.size(); ++i)
            matrix.at(i).at(i) = -1;
    tmp.examsCollisionsAbove->resize(15,
                                std::vector<std::vector<int>>(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0)));
    tmp.examCollisionExams->resize(NUMBER_OF_EXAMS);
    tmp.examDegree->resize(NUMBER_OF_EXAMS, 0);
    // fill data structures
    for (auto &student: tmp.enrollment.value()) {
        for (auto &exam: student) {
            //if exams didn't have attribute size it could be created with this line of code ++manipulator_->examSize.at(exam); instead
            for (auto &otherExam: student) {
                if (exam == otherExam)
                    continue;
                ++tmp.examsCollisions->at(exam).at(otherExam);
                for (size_t s = student.size(); s < tmp.examsCollisionsLimit->size(); ++s) {
                    ++tmp.examsCollisionsLimit->at(s).at(exam).at(otherExam);
                    ++tmp.examsCollisionsAbove->at(s).at(exam).at(otherExam);
                }
                tmp.examCollisionExams->at(exam).insert(otherExam);
                ++tmp.examDegree->at(exam);
            }
        }
    }
    return *this;
}

ExamTTDataBuilder ExamTTDataBuilder::createAllPossibleRoomCombinationsForEachExam() {
    const auto NUMBER_OF_EXAMS = tmp.examID->size();
    tmp.examsPossibleRoomCombinations = std::vector<std::vector<std::pair<std::set<int>, int>>>(NUMBER_OF_EXAMS);
    for (int examIndex = 0; examIndex < NUMBER_OF_EXAMS; ++examIndex) {
        auto validRooms = VectorUtils::getIndexesWherePredicate(tmp.examRoomsValidity.value().at(examIndex), [](const int& a){return a == 1;});
        auto binIndexAndSize = VectorUtils::getSortedValuesUsingValueAsIndexInSecond(validRooms, tmp.roomSize.value(), [](std::pair<int, int> a, std::pair<int, int> b){return a.second > b.second;});
        auto roomCombos = VectorUtils::binPackingAllBins(tmp.examSize.value().at(examIndex), binIndexAndSize);
        VectorUtils::sortBinResult(roomCombos);
        tmp.examsPossibleRoomCombinations.value().at(examIndex) = roomCombos;
    }
    return *this;
}

std::shared_ptr<ExamTTData> ExamTTDataBuilder::build() {
    return std::make_shared<ExamTTData>(tmp);
}

void ExamTTDataBuilder::createCollisionMatrixLimitEnrolment() {
    // set up data structures
    const auto NUMBER_OF_EXAMS = tmp.examID->size();
    tmp.examsCollisionsLimit->resize(15,
                                std::vector<std::vector<int>>(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0)));
    for (auto &matrix: tmp.examsCollisionsLimit.value())
        for (int i = 0; i < matrix.size(); ++i)
            matrix.at(i).at(i) = -1;

    tmp.examsCollisionsAbove->resize(15,
                                std::vector<std::vector<int>>(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0)));
    for (auto &matrix: tmp.examsCollisionsAbove.value())
        for (int i = 0; i < matrix.size(); ++i)
            matrix.at(i).at(i) = -1;
    for (auto &student: tmp.enrollment.value()) {
        for (auto &exam: student) {
            for (auto &otherExam: student) {
                if (exam == otherExam)
                    continue;
                for (size_t i = student.size(); i < tmp.examsCollisionsLimit->size(); ++i) {
                    ++tmp.examsCollisionsLimit->at(i).at(exam).at(otherExam);
                    if (i + 1 < tmp.examsCollisionsLimit->size())
                        ++tmp.examsCollisionsAbove->at(i + 1).at(exam).at(otherExam);
                }
            }
        }
    }
}

/*void ExamTTDataBuilder::setUpSolutionDependentData() {
    const auto NUMBER_OF_PERIODS = periodID->size();
    const auto NUMBER_OF_EXAMS = examID->size();
    // add all exams unscheduled
    examPeriod.resize(NUMBER_OF_EXAMS, -1);
    // add all exams empty
    examRooms.resize(NUMBER_OF_EXAMS);
    // add all periods with all exams and no collisions
    periodExamCollisions.resize(NUMBER_OF_PERIODS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    // add all exams with no period collisions
    examDegreeOfSaturation.resize(NUMBER_OF_EXAMS, 0);
    //add all periods empty
    periodExams.resize(NUMBER_OF_PERIODS);
}

void ExamTTDataBuilder::processSolutionExamPeriod(const std::vector<std::vector<std::string>> &examPeriod) {
    for (const auto &row: examPeriod) {
        int examIndex = VectorUtils::indexForValue(examID.value(), row.at(0));
        if (examIndex == -1)
            continue;
        int periodIndex = VectorUtils::indexForValue(periodID.value(), row.at(1));
        if (periodIndex == -1)
            continue;
        this->examPeriod.at(examIndex) = periodIndex;
    }
}

void ExamTTDataBuilder::processSolutionExamRooms(const std::vector<std::vector<std::string>> &examRooms) {
    for (const auto &row: examRooms) {
        int examIndex = VectorUtils::indexForValue(examID.value(), row.at(0));
        if (examIndex == -1)
            continue;
        int roomIndex = VectorUtils::indexForValue(roomID.value(), row.at(1));
        if (roomIndex == -1)
            continue;
        this->examRooms.at(examIndex).insert(roomIndex);
    }
}*/


RoomType ExamTTDataBuilder::stringToRoomType(const std::string &str) {
    if (str == "online")
        return RoomType::Online;
    if (str == "einzeln")
        return RoomType::Single;
    if (str == "normal")
        return RoomType::Normal;
    if (str == "extern")
        return RoomType::External;
    return RoomType::Unknown;
}


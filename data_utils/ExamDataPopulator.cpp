//
// Created by hansk on 04.06.2024.
//

#include <stdexcept>
#include "ExamDataPopulator.h"
#include "vectorUtils/VectorUtils.h"

void ExamDataPopulator::processPeriods(const std::vector<std::vector<std::string>> &periods) {
    for (auto &period: periods) {
        if (period.size() < 5)
            return;
        examData->periodID.push_back(std::stoi(period.at(0)));
        examData->periodDay.push_back(std::stoi(period.at(1)));
        examData->periodSlot.push_back(std::stoi(period.at(2)));
        examData->periodWeek.push_back(std::stoi(period.at(3)));
        examData->periodDate.push_back(period.at(4));
    }
}

void ExamDataPopulator::processRooms(const std::vector<std::vector<std::string>> &rooms) {
    for (auto &room: rooms) {
        if (room.size() < 4)
            return;
        examData->roomID.push_back(std::stoi(room.at(0)));
        examData->roomName.push_back(room.at(1));
        examData->roomSize.push_back(std::stoi(room.at(2)));
        examData->roomType.push_back(stringToRoomType(room.at(3)));
    }
}

void ExamDataPopulator::processRoomsValidPeriods(const std::vector<std::vector<std::string>> &roomsPeriods) {
    // add all roomsPeriods with initially all periods invalid with value 0
    examData->roomPeriodsValidity.resize(examData->roomID.size(), std::vector<int>(examData->periodID.size(), 0));
    // add all periods with initially all roomsPeriods invalid with value -1
    examData->periodRoomsAvailability.resize(examData->periodID.size(), std::vector<int>(examData->roomID.size(), -1));
    for (auto &row: roomsPeriods) {
        auto room = VectorUtils::indexForValue(examData->roomID, row.at(0));
        if (room == -1)
            continue;
        auto period = VectorUtils::indexForValue(examData->periodID, row.at(1));
        if (period == -1)
            continue;
        //validate this period for the room
        examData->roomPeriodsValidity.at(room).at(period) = 1;
        //mark the room available (and valid) in this period // necessary
        examData->periodRoomsAvailability.at(period).at(room) = 1;
    }
}

void ExamDataPopulator::processExams(const std::vector<std::vector<std::string>> &exams) {
    for (auto &exam: exams) {
        if (exam.size() < 3)
            return;
        examData->examID.push_back(std::stoi(exam.at(0)));
        examData->examName.push_back(exam.at(1));
        examData->examSize.push_back(std::stoi(exam.at(2))); //necessary but could easily be summed up from enrollment
    }
}

// necessary
void ExamDataPopulator::processExamValidPeriods(const std::vector<std::vector<std::string>> &examsPeriods) {
    // add all examsPeriods with initially all periods invalid
    examData->examPeriodsValidity.resize(examData->examID.size(), std::vector<int>(examData->periodID.size(), 0));
    for (auto &row: examsPeriods) {
        auto exam = VectorUtils::indexForValue(examData->examID, row.at(0));
        if (exam == -1)
            continue;
        auto period = VectorUtils::indexForValue(examData->periodID, row.at(1));
        if (period == -1)
            continue;
        examData->examPeriodsValidity.at(exam).at(period) = 1; //validate this row for the exam
    }
}

// necessary
void ExamDataPopulator::processExamsValidRooms(const std::vector<std::vector<std::string>> &examsRooms) {
    // add all examsRooms with initially all rooms invalid
    examData->examRoomsValidity.resize(examData->examID.size(), std::vector<int>(examData->roomID.size(), 0));
    for (auto &row: examsRooms) {
        auto exam = VectorUtils::indexForValue(examData->examID, row.at(0));
        if (exam == -1)
            continue;
        auto room = VectorUtils::indexForValue(examData->roomID, row.at(1));
        if (room == -1)
            continue;
        examData->examRoomsValidity.at(exam).at(room) = 1; //validate this row for the exam
    }
}

// necessary
void ExamDataPopulator::processStudentsExams(const std::vector<std::vector<std::string>> &studentsExams) {
    std::string prev = "-1";
    for (auto &row: studentsExams) {
        if (row.at(0) == "-1")
            continue;
        if (row.at(0) != prev) {
            prev = row.at(0);
            examData->enrollment.emplace_back(); //add a student without enrolled row
        }
        auto exam = VectorUtils::indexForValue(examData->examID, row.at(1));
        if (exam == -1)
            continue;
        examData->enrollment.back().insert(exam);
    }
}

// necessary
void ExamDataPopulator::processSamePeriods(const std::vector<std::vector<std::string>> &samePeriods) {
    // We use this to check if we encountered a new set of connected exams
    std::string prev;
    // In this we gather all sets of connected exams before processing them
    std::vector<std::set<std::string>> gatheredSamePeriod;
    // Set up the vector
    examData->examSamePeriod.resize(examData->examID.size());

    // Gather all sets of connected exams
    for (auto &row: samePeriods) {
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
                int examIndex = VectorUtils::indexForValue(examData->examID, exam);
                if (examIndex == -1)
                    continue;
                int otherExamIndex = VectorUtils::indexForValue(examData->examID, otherExam);
                if (otherExamIndex == -1)
                    continue;
                examData->examSamePeriod.at(examIndex).insert(otherExamIndex);
            }
        }
    }
}

void ExamDataPopulator::createCollisionsFromEnrollment() {
    const auto NUMBER_OF_EXAMS = examData->examID.size();
    // set up data structures
    examData->examsCollisions.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisions.size(); ++i)
        examData->examsCollisions.at(i).at(i) = -1;
    examData->examCollisionExams.resize(NUMBER_OF_EXAMS);
    examData->examDegree.resize(NUMBER_OF_EXAMS, 0);
    // fill data structures
    for (auto &student: examData->enrollment) {
        for (auto &exam: student) {
            //if exams didn't have attribute size it could be created with this line of code ++manipulator_->examSize.at(exam); instead
            for (auto &otherExam: student) {
                if (exam == otherExam)
                    continue;
                ++examData->examsCollisions.at(exam).at(otherExam);
                examData->examCollisionExams.at(exam).insert(otherExam);
                ++examData->examDegree.at(exam);
            }
        }
    }
}

void ExamDataPopulator::createCollisionMatrixLimitEnrolment() {
    const auto NUMBER_OF_EXAMS = examData->examID.size();
    // set up data structures
    examData->examsCollisionsLimit7.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit7.size(); ++i)
        examData->examsCollisionsLimit7.at(i).at(i) = -1;
    examData->examsCollisionsBeyond7.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsBeyond7.size(); ++i)
        examData->examsCollisionsBeyond7.at(i).at(i) = -1;
    examData->examsCollisionsLimit8.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit8.size(); ++i)
        examData->examsCollisionsLimit8.at(i).at(i) = -1;
    examData->examsCollisionsLimit9.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit9.size(); ++i)
        examData->examsCollisionsLimit9.at(i).at(i) = -1;
    examData->examsCollisionsLimit10.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit10.size(); ++i)
        examData->examsCollisionsLimit10.at(i).at(i) = -1;
    examData->examsCollisionsLimit11.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit11.size(); ++i)
        examData->examsCollisionsLimit11.at(i).at(i) = -1;
    examData->examsCollisionsLimit12.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit12.size(); ++i)
        examData->examsCollisionsLimit12.at(i).at(i) = -1;
    examData->examsCollisionsLimit13.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit13.size(); ++i)
        examData->examsCollisionsLimit13.at(i).at(i) = -1;
    examData->examsCollisionsLimit14.resize(NUMBER_OF_EXAMS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    for (int i = 0; i < examData->examsCollisionsLimit14.size(); ++i)
        examData->examsCollisionsLimit14.at(i).at(i) = -1;
    for (auto &student:examData->enrollment) {
        for (auto &exam:student) {
            for (auto &otherExam:student) {
                if(exam == otherExam)
                    continue;
                if(student.size() <= 7)
                    ++examData->examsCollisionsLimit7.at(exam).at(otherExam);
                if(student.size() > 7)
                    ++examData->examsCollisionsBeyond7.at(exam).at(otherExam);
                if(student.size() <= 8)
                    ++examData->examsCollisionsLimit8.at(exam).at(otherExam);
                if(student.size() <= 9)
                    ++examData->examsCollisionsLimit9.at(exam).at(otherExam);
                if(student.size() <= 10)
                    ++examData->examsCollisionsLimit10.at(exam).at(otherExam);
                if(student.size() <= 11)
                    ++examData->examsCollisionsLimit11.at(exam).at(otherExam);
                if(student.size() <= 12)
                    ++examData->examsCollisionsLimit12.at(exam).at(otherExam);
                if(student.size() <= 13)
                    ++examData->examsCollisionsLimit13.at(exam).at(otherExam);
                if(student.size() <= 14)
                    ++examData->examsCollisionsLimit14.at(exam).at(otherExam);
            }
        }
    }
}

void ExamDataPopulator::setUpSolutionDependentData() {
    const auto NUMBER_OF_PERIODS = examData->periodID.size();
    const auto NUMBER_OF_EXAMS = examData->examID.size();
    // add all exams unscheduled
    examData->examPeriod.resize(NUMBER_OF_EXAMS, -1);
    // add all exams empty
    examData->examRooms.resize(NUMBER_OF_EXAMS);
    // add all periods with all exams and no collisions
    examData->periodExamCollisions.resize(NUMBER_OF_PERIODS, std::vector<int>(NUMBER_OF_EXAMS, 0));
    // add all exams with no period collisions
    examData->examDegreeOfSaturation.resize(NUMBER_OF_EXAMS, 0);
    //add all periods empty
    examData->periodExams.resize(NUMBER_OF_PERIODS);
}

void ExamDataPopulator::processSolutionExamPeriod(const std::vector<std::vector<std::string>> &examPeriod) {
    for (const auto &row: examPeriod) {
        int examIndex = VectorUtils::indexForValue(examData->examID, row.at(0));
        if (examIndex == -1)
            continue;
        int periodIndex = VectorUtils::indexForValue(examData->periodID, row.at(1));
        if (periodIndex == -1)
            continue;
    }
}

void ExamDataPopulator::processSolutionExamRooms(const std::vector<std::vector<std::string>> &examRooms) {
    for (const auto &row: examRooms) {
        int examIndex = VectorUtils::indexForValue(examData->examID, row.at(0));
        if (examIndex == -1)
            continue;
        int roomIndex = VectorUtils::indexForValue(examData->roomID, row.at(1));
        if (roomIndex == -1)
            continue;
        examData->examRooms.at(examIndex).insert(roomIndex);
    }
}


ExamTTData::RoomType ExamDataPopulator::stringToRoomType(const std::string &str) {
    if (str == "online")
        return ExamTTData::RoomType::Online;
    if (str == "einzeln")
        return ExamTTData::RoomType::Single;
    if (str == "normal")
        return ExamTTData::RoomType::Normal;
    if (str == "extern")
        return ExamTTData::RoomType::External;
    return ExamTTData::RoomType::Unknown;
}


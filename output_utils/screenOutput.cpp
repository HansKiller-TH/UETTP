//
// Created by hansk on 11.05.2024.
//

#include <algorithm>
#include "screenOutput.h"
#include "Algorithm/Evaluation.h"

void screenOutput::setOut(const std::set<int> &s, std::string name) {
    std::cout << name << std::endl;
    for (auto item: s) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void screenOutput::vectorOut(std::vector<int> vec, std::string name) {
    std::cout << name << std::endl;
    for (auto item: vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    std::cout << " l:" << std::to_string(vec.size()) << std::endl;
}

void screenOutput::vectorOut(std::vector<std::string> vec, std::string name) {
    std::cout << name << std::endl;
    for (auto item: vec) {
        std::cout << item << std::endl;
    }
    std::cout << " l:" << std::to_string(vec.size()) << std::endl;
}

void screenOutput::vectorOut(std::vector<std::vector<int>> vec, std::string name) {
    std::cout << name << std::endl;
    for (const auto &row: vec) {
        for (auto line: row) {
            std::cout << line << " ";
        }
        std::cout << " l:" << std::to_string(row.size());
        std::cout << std::endl;
    }
    std::cout << " lO:" << std::to_string(vec.size()) << std::endl;
}
void screenOutput::vectorOut(std::vector<std::set<int>> &vec, const std::string& name) {
    std::cout << name << std::endl;
    for (const auto &row: vec) {
        for (auto line: row) {
            std::cout << line << " ";
        }
        std::cout << " l:" << std::to_string(row.size());
        std::cout << std::endl;
    }
    std::cout << " lO:" << std::to_string(vec.size()) << std::endl;
}
void screenOutput::vectorOut(std::vector<ExamTTData::RoomType> &vec, const std::string& name) {
    std::cout << name << std::endl;
    for (const auto &row: vec) {
            std::cout << std::to_string((int)row) << " ";
        std::cout << std::endl;
    }
    std::cout << " l:" << std::to_string(vec.size()) << std::endl;
}
void screenOutput::solutionOut(ExamTTData &data) {
    std::vector<std::pair<int, int>> indexedExamPeriod;
    for (size_t i = 0; i < data.examPeriod.size(); ++i) {
        indexedExamPeriod.emplace_back(data.examPeriod[i], i);
    }
    std::sort(indexedExamPeriod.begin(), indexedExamPeriod.end());

    for (const auto &pair: indexedExamPeriod) {
        int period = pair.first;
        int exam = pair.second;
        std::cout << "exam:" << exam << " '" << data.examName.at(exam) << "', id:" << data.examID.at(exam);
        std::cout << ", period:" << period;
        if (period != -1) {
            std::cout << ", day:" << data.periodDay.at(period);
            std::cout << ", slot:" << data.periodSlot.at(period);
        }
        std::cout << std::endl;
        std::cout << "size:" << data.examSize.at(exam);
        std::cout << ", rooms: ";
        for (auto &roomIndex: data.examRooms.at(exam)) {
            std::cout << "[" << std::to_string(roomIndex) << "]" << data.roomName.at(roomIndex) << "("
                      << data.roomSize.at(roomIndex) << ") ";
        }
        std::cout << std::endl;
    }

    int roomCount = 0;
    for (int i = 0; i < data.examRooms.size(); ++i) {
        roomCount += data.examRooms.at(i).size();
    }
    std::cout << "Number of rooms used: " << roomCount << std::endl;
    std::cout << "Cost: " << Evaluation::calculateCost(data) << std::endl;
}

void screenOutput::solutionDifference(const ExamTTData &data1, const ExamTTData &data2) {
    for (int exam = 0; exam < data1.examPeriod.size(); ++exam) {
        if (data1.examPeriod.at(exam) != data2.examPeriod.at(exam))
            std::cout << "Exam " << exam << " has different period: " << data1.examPeriod.at(exam) << " -> "
                      << data2.examPeriod.at(exam) << std::endl;
    }
    for (int exam = 0; exam < data1.examRooms.size(); ++exam) {
        if (data1.examRooms.at(exam) != data2.examRooms.at(exam)) {
            std::cout << "Exam " << exam << " has different rooms: ";
            for (auto room: data1.examRooms.at(exam)) {
                std::cout << room << " ";
            }
            std::cout << "-> ";
            for (auto room: data2.examRooms.at(exam)) {
                std::cout << room << " ";
            }
            std::cout << std::endl;
        }
    }
}

void screenOutput::printExamData(ExamTTData &examData) {
    std::cout << examData.filePath << std::endl;
    std::cout << examData.timeStamp << std::endl;
    vectorOut(examData.periodID,"periodID");
    vectorOut(examData.periodDay,"periodDay");
    vectorOut(examData.periodSlot, "periodSlot");
    vectorOut(examData.periodWeek, "periodWeek");
    vectorOut(examData.periodDate, "periodDate");
    vectorOut(examData.roomID, "roomID");
    vectorOut(examData.roomName, "roomName");
    vectorOut(examData.roomSize, "roomSize");
    vectorOut(examData.roomType, "roomType");
    vectorOut(examData.roomPeriodsValidity, "roomPeriodsValidity");
    vectorOut(examData.examID, "examID");
    vectorOut(examData.examName, "examName");
    vectorOut(examData.examSize, "examSize");
    vectorOut(examData.examPeriodsValidity, "examPeriodsValidity");
    vectorOut(examData.examRoomsValidity, "examRoomsValidity");
    vectorOut(examData.enrollment, "enrollment");
    vectorOut(examData.examsCollisions, "examsCollisions");
    vectorOut(examData.examCollisionExams, "examCollisionExams");
    vectorOut(examData.examSamePeriod, "examSamePeriod");
    vectorOut(examData.examRooms, "examRooms");
    vectorOut(examData.examPeriod, "examPeriod");
    vectorOut(examData.periodRoomsAvailability, "periodRoomsAvailability");
    vectorOut(examData.periodExamCollisions, "periodExamCollisions");
    vectorOut(examData.periodExams, "periodExams");
    vectorOut(examData.examDegreeOfSaturation, "examDegreeOfSaturation");
    vectorOut(examData.examDegree, "examDegree");
}

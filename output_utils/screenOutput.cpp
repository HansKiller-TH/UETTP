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
void screenOutput::vectorOut(const std::vector<std::set<int>> &vec, const std::string& name) {
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
void screenOutput::vectorOut(const std::vector<RoomType> &vec, const std::string& name) {
    std::cout << name << std::endl;
    for (const auto &row: vec) {
            std::cout << std::to_string((int)row) << " ";
        std::cout << std::endl;
    }
    std::cout << " l:" << std::to_string(vec.size()) << std::endl;
}
void screenOutput::solutionOut(ExamTTSolution &data) {
    std::vector<std::pair<int, int>> indexedExamPeriod;
    for (size_t i = 0; i < data.examPeriod.size(); ++i) {
        indexedExamPeriod.emplace_back(data.examPeriod[i], i);
    }
    std::sort(indexedExamPeriod.begin(), indexedExamPeriod.end());

    for (const auto &pair: indexedExamPeriod) {
        int period = pair.first;
        int exam = pair.second;
        std::cout << "exam:" << exam << " '" << data.examData->examName.at(exam) << "', id:" << data.examData->examID.at(exam);
        std::cout << ", period:" << period;
        if (period != -1) {
            std::cout << ", day:" << data.examData->periodDay.at(period);
            std::cout << ", slot:" << data.examData->periodSlot.at(period);
        }
        std::cout << std::endl;
        std::cout << "size:" << data.examData->examSize.at(exam);
        std::cout << ", rooms: ";
        for (auto &roomIndex: data.examRooms.at(exam)) {
            std::cout << "[" << std::to_string(roomIndex) << "]" << data.examData->roomName.at(roomIndex) << "("
                      << data.examData->roomSize.at(roomIndex) << ") ";
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

void screenOutput::solutionDifference(const ExamTTSolution &data1, const ExamTTSolution &data2) {
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

void screenOutput::printExamData(ExamTTSolution &data) {
    std::cout << data.examData->filePath << std::endl;
    std::cout << data.examData->timeStamp << std::endl;
    vectorOut(data.examData->periodID, "periodID");
    vectorOut(data.examData->periodDay, "periodDay");
    vectorOut(data.examData->periodSlot, "periodSlot");
    vectorOut(data.examData->periodWeek, "periodWeek");
    vectorOut(data.examData->periodDate, "periodDate");
    vectorOut(data.examData->roomID, "roomID");
    vectorOut(data.examData->roomName, "roomName");
    vectorOut(data.examData->roomSize, "roomSize");
    vectorOut(data.examData->roomType, "roomType");
    vectorOut(data.examData->roomPeriodsValidity, "roomPeriodsValidity");
    vectorOut(data.examData->examID, "examID");
    vectorOut(data.examData->examName, "examName");
    vectorOut(data.examData->examSize, "examSize");
    vectorOut(data.examData->examPeriodsValidity, "examPeriodsValidity");
    vectorOut(data.examData->examRoomsValidity, "examRoomsValidity");
    vectorOut(data.examData->enrollment, "enrollment");
    vectorOut(data.examData->examsCollisions, "examsCollisions");
    vectorOut(data.examData->examCollisionExams, "examCollisionExams");
    vectorOut(data.examData->examSamePeriod, "examSamePeriod");
    vectorOut(data.examRooms, "examRooms");
    vectorOut(data.examPeriod, "examPeriod");
    vectorOut(data.periodRoomsAvailability, "periodRoomsAvailability");
    vectorOut(data.periodExamCollisions, "periodExamCollisions");
    vectorOut(data.periodExams, "periodExams");
    vectorOut(data.examDegreeOfSaturation, "examDegreeOfSaturation");
    vectorOut(data.examData->examDegree, "examDegree");
}

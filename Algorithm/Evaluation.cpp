//
// Created by hansk on 11.05.2024.
//

#include <iostream>
#include <sstream>
#include "Evaluation.h"
#include "data_utils/ExamTTSolution.h"

bool Evaluation::isFeasible(const ExamTTSolution &data) {
    bool result = true;
    if (hasSpreadCollision(data))
        result = false;
    if (hasRoomCollision(data))
        result = false;
    if (hasInvalidPeriod(data))
        result = false;
    return result;
}

int Evaluation::calculateCost(ExamTTSolution &data) {
    int cost = 0;
    cost += spreadCost(data);
    cost += roomCost(data);
    cost += periodCost(data);
    return cost;
}
int Evaluation::calculateAltCost(ExamTTSolution &data) {
    int cost = 0;
    cost += Evaluation::spreadCostLimit(data);
    cost += Evaluation::roomCost(data);
    cost += Evaluation::periodCost(data);
    return cost;
}

int Evaluation::spreadCost(const ExamTTSolution &data) {
    int cost = 0;
    for (int exam = 0; exam < data.examData->examsCollisions.size(); ++exam) {
        for (int otherEx = exam + 1; otherEx < data.examData->examsCollisions.size(); ++otherEx) {
            int collAmount = data.examData->examsCollisions.at(exam).at(otherEx);
            if (collAmount < 1)
                continue;
            int examDay = data.examData->periodDay.at(data.examPeriod.at(exam));
            int otherExDay = data.examData->periodDay.at(data.examPeriod.at(otherEx));
            int diff = std::abs(examDay - otherExDay);
            if (diff > 1)
                continue;
            cost += diff == 1 ? collAmount * P_EXAM_CONSECUTIVE_DAYS : collAmount * P_EXAM_SAME_DAY;
        }
    }
    return cost;
}
int Evaluation::spreadCostLimit(const ExamTTSolution &data) {
    int cost = 0;
    for (int exam = 0; exam < data.examData->examsCollisionsLimit7.size(); ++exam) {
        for (int otherEx = exam + 1; otherEx < data.examData->examsCollisionsLimit7.size(); ++otherEx) {
            int collAmount = data.examData->examsCollisionsLimit7.at(exam).at(otherEx);
            if (collAmount < 1)
                continue;
            int examDay = data.examData->periodDay.at(data.examPeriod.at(exam));
            int otherExDay = data.examData->periodDay.at(data.examPeriod.at(otherEx));
            int diff = std::abs(examDay - otherExDay);
            if (diff > 1)
                continue;
            cost += diff == 1 ? collAmount * P_EXAM_CONSECUTIVE_DAYS : collAmount * P_EXAM_SAME_DAY;
        }
    }
    return cost;
}

int Evaluation::spreadCostBeyond(const ExamTTSolution &data) {
    int cost = 0;
    for (int exam = 0; exam < data.examData->examsCollisionsBeyond7.size(); ++exam) {
        for (int otherEx = exam + 1; otherEx < data.examData->examsCollisionsBeyond7.size(); ++otherEx) {
            int collAmount = data.examData->examsCollisionsBeyond7.at(exam).at(otherEx);
            if (collAmount < 1)
                continue;
            int examDay = data.examData->periodDay.at(data.examPeriod.at(exam));
            int otherExDay = data.examData->periodDay.at(data.examPeriod.at(otherEx));
            int diff = std::abs(examDay - otherExDay);
            if (diff > 1)
                continue;
            cost += diff == 1 ? collAmount * P_EXAM_CONSECUTIVE_DAYS : collAmount * P_EXAM_SAME_DAY;
        }
    }
    return cost;
}

int Evaluation::roomCost(const ExamTTSolution &data) {
    // Better set a per room penalty to the periods in the data file to be parsed to the ExamTTData obj
    int cost = 0;
    for (int exam = 0; exam < data.examPeriod.size(); ++exam) {
        int tmp = 0;
        int period = data.examPeriod.at(exam);
        if(period == -1)
            continue;
        // Determine if that period is not on a saturday and hence continue
        // Saturday only has two periods. Sunday has none.
        if (period + 2 < data.examData->periodDay.size() && data.examData->periodDay.at(period) == data.examData->periodDay.at(period + 2))
            continue;
        if (period - 2 >= 0 && data.examData->periodDay.at(period) == data.examData->periodDay.at(period - 2))
            continue;
        // We believe this period to be on a saturday now
        for (auto &room: data.examRooms.at(exam)) {
            // Scarce external rooms must be used when available regardless of weekday and
            // thus these or any other rooms occupied by that exam mustn't be penalized
            if (data.examData->roomType.at(room) == ExamTTData::RoomType::External) {
                tmp = 0;
                break;
            }
            tmp += P_ROOM_SATURDAY;
        }
        cost += tmp;
    }
    return cost;
}

int Evaluation::periodCost(const ExamTTSolution &data) {
    // Better set a per-student penalty to the periods in the data file to be parsed to the ExamTTData obj
    int cost = 0;
    const int DELTA = data.examData->periodDay.back() - 6;
    for (int exam = 0; exam < data.examPeriod.size(); ++exam) {
        const int PERIOD = data.examPeriod.at(exam);
        const int DAY = data.examData->periodDay.at(PERIOD);
        const int DAY_OF_LAST_WEEK = DAY - DELTA;
        if (DAY_OF_LAST_WEEK < 0)
            continue;
        cost += data.examData->examSize.at(exam) * P_EXAM_DAYS_OF_LAST_WEEK.at(DAY_OF_LAST_WEEK);
    }
    return cost;
}

bool Evaluation::hasInvalidPeriod(const ExamTTSolution &data) {
    bool result = false;
    for (int exam = 0; exam < data.examPeriod.size(); ++exam) {
        int period = data.examPeriod.at(exam);
        if (data.examData->examPeriodsValidity.at(exam).at(period) < 1) {
            std::cout << "Invalid period: " << period << " for exam: " << exam << std::endl;
            result = true;
        }
    }
    return result;
}

bool Evaluation::hasRoomCollision(const ExamTTSolution &data) {
    bool result = false;
    for (int exam = 0; exam < data.examPeriod.size() - 1; ++exam) {
        for (int otherEx = exam + 1; otherEx < data.examPeriod.size(); ++otherEx) {
            auto period = data.examPeriod.at(exam);
            auto otherPeriod = data.examPeriod.at(otherEx);
            bool samePeriod = period == otherPeriod;
            if (!samePeriod)
                continue;
            for (auto &roomIndex: data.examRooms.at(exam)) {
                for (auto &otherIndex: data.examRooms.at(otherEx)) {
                    bool sameRoom = roomIndex == otherIndex;
                    if (sameRoom) {
                        std::cout << "Room " << std::to_string(roomIndex) << " p:"
                                  << std::to_string(data.examPeriod.at(exam)) << " collision: ex:"
                                  << std::to_string(exam) << " ex:" << std::to_string(otherEx) << std::endl;
                        result = true;
                    }
                }
            }
        }
    }
    return result;
}

bool Evaluation::hasSpreadCollision(const ExamTTSolution &data) {
    bool result = false;
    for (int exam = 0; exam < data.examData->examsCollisions.size(); ++exam) {
        for (int otherEx = exam + 1; otherEx < data.examData->examsCollisions.size(); ++otherEx) {
            if (data.examData->examsCollisions.at(exam).at(otherEx) <= 0)
                continue;
            int period = data.examPeriod.at(exam);
            int otherPeriod = data.examPeriod.at(otherEx);
            int diffP = std::abs(period - otherPeriod);
            if (diffP >= 2)
                continue;
            int day = data.examData->periodDay.at(period);
            int otherDay = data.examData->periodDay.at(otherPeriod);
            if (day == otherDay) {
                std::cout << "Spread collision between exams " << exam << " and " << otherEx << " in period "
                          << period << " and " << otherPeriod
                          << std::endl;
                result = true;
            }
        }
    }
    return result;
}

std::pair<std::vector<std::multiset<int>>, std::vector<std::multiset<int>>> Evaluation::spreadPerStudent(
        std::shared_ptr<ExamTTSolution> data) {
    auto enroll = data->examData->enrollment;
    std::vector<std::multiset<int>> sameDay(15);
    std::vector<std::multiset<int>> consecutiveDays(15);
    for (auto &student:enroll) {
        int same = 0;
        int consecutive = 0;
        for (auto e = student.begin(); e != student.end(); ++e) {
            for (auto o = std::next(e); o != student.end(); ++o) {
                int examDay = data->examData->periodDay.at(data->examPeriod.at(*e));
                int otherExDay = data->examData->periodDay.at(data->examPeriod.at(*o));
                int diff = std::abs(examDay - otherExDay);
                if (diff > 1)
                    continue;
                if(diff == 0){
                    ++same;
                }
                else if(diff == 1){
                    ++consecutive;
                }
            }
        }
        if(same)
            sameDay.at(student.size()).insert(same);
        if(consecutive)
            consecutiveDays.at(student.size()).insert(consecutive);
    }
    return {sameDay,consecutiveDays};
}

std::vector<int> Evaluation::roomsOnSaturday(std::shared_ptr<ExamTTSolution> data) {
    std::vector<int> result;
    for (int exam = 0; exam < data->examPeriod.size(); ++exam) {
        int tmp = 0;
        int period = data->examPeriod.at(exam);
        if(period == -1)
            continue;
        // Determine if that period is not on a saturday and hence continue
        // Saturday only has two periods. Sunday has none.
        if (period + 2 < data->examData->periodDay.size() && data->examData->periodDay.at(period) == data->examData->periodDay.at(period + 2))
            continue;
        if (period - 2 >= 0 && data->examData->periodDay.at(period) == data->examData->periodDay.at(period - 2))
            continue;
        // We believe this period to be on a saturday now
        for (auto &room: data->examRooms.at(exam)) {
            if(room >= result.size())
                result.resize(room+1);
            result.at(room)++;
        }
    }
    return result;
}

std::vector<int> Evaluation::studentsLastWeek(std::shared_ptr<ExamTTSolution> data) {
    std::vector<int> result;
    const int DELTA = data->examData->periodDay.back() - 6;
    for (int exam = 0; exam < data->examPeriod.size(); ++exam) {
        const int PERIOD = data->examPeriod.at(exam);
        const int DAY = data->examData->periodDay.at(PERIOD);
        const int DAY_OF_LAST_WEEK = DAY - DELTA;
        if (DAY_OF_LAST_WEEK < 0)
            continue;
        if(DAY_OF_LAST_WEEK >= result.size())
            result.resize(DAY_OF_LAST_WEEK+1);
        result.at(DAY_OF_LAST_WEEK) += data->examData->examSize.at(exam);
    }
    return result;
}

double Evaluation::getCollisionDensity(const std::vector<std::vector<int>> &adjacencyMatrix) {
    double numEdges = 0;
    const auto numExams = static_cast<double>(adjacencyMatrix.size());
    const double allVerticesPairs = (numExams * (numExams - 1.0)) / 2.0;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        for (int j = i + 1; j < adjacencyMatrix.at(i).size(); ++j) {
            if(adjacencyMatrix.at(i).at(j) > 0)
                ++numEdges;
        }
    }
    return numEdges / allVerticesPairs;
}

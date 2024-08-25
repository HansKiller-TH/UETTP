//
// Created by hansk on 11.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EVALUATION_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EVALUATION_H


#include <memory>
#include <array>
#include "data_utils/ExamTTData.h"

class Evaluation {
public:
    static std::pair<std::vector<std::multiset<int>>, std::vector<std::multiset<int>>> spreadPerStudent(std::shared_ptr<ExamTTData> data);

    static std::vector<int> roomsOnSaturday(std::shared_ptr<ExamTTData> data);

    static std::vector<int> studentsLastWeek(std::shared_ptr<ExamTTData> data);

    static bool isFeasible(const ExamTTData &data);

    static int calculateCost(ExamTTData &data);

    static int calculateAltCost(ExamTTData &data);

    static int spreadCost(const ExamTTData &data);

    static int roomCost(const ExamTTData &data);

    static int periodCost(const ExamTTData &data);

    static int spreadCostLimit(const ExamTTData &data);

    static int spreadCostBeyond(const ExamTTData &data);

    static double getCollisionDensity(const std::vector<std::vector<int>> &adjacencyMatrix);

private:
    static const int P_EXAM_CONSECUTIVE_DAYS = 1;
    static const int P_EXAM_SAME_DAY = 10;
    constexpr static const std::array<int, 7> P_EXAM_DAYS_OF_LAST_WEEK = {1, 2, 4, 8, 16, 32, 64};
    static const int P_ROOM_SATURDAY = 256;

    static bool hasInvalidPeriod(const ExamTTData &data);

    static bool hasRoomCollision(const ExamTTData &data);

    static bool hasSpreadCollision(const ExamTTData &data);

};

#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EVALUATION_H

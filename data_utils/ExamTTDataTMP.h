//
// Created by hansk on 30.08.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATATMP_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATATMP_H


#include <string>
#include <vector>
#include <optional>
#include "ExamTTData.h"
#include "RoomType.h"

struct ExamTTDataTMP {
    std::optional<std::string> filePath;
    std::optional<std::string> timeStamp;
    std::optional<std::string> dataSet;
    std::optional<std::vector<int>> periodID;
    std::optional<std::vector<int>> periodDay;
    std::optional<std::vector<int>> periodSlot;
    std::optional<std::vector<int>> periodWeek;
    std::optional<std::vector<std::string>> periodDate;
    std::optional<std::vector<int>> roomID; //use id when writing timetable solution_ to file
    std::optional<std::vector<std::string>> roomName; //for convenience only
    std::optional<std::vector<int>> roomSize;
    std::optional<std::vector<RoomType>> roomType;
    std::optional<std::vector<std::vector<int>>> roomPeriodsValidity; //inconvenient. not used currently
    std::optional<std::vector<int>> examID; //use id when writing timetable solution_ to file
    std::optional<std::vector<std::string>> examName; //for convenience only
    std::optional<std::vector<int>> examSize;
    std::optional<std::vector<std::vector<int>>> examPeriodsValidity;
    std::optional<std::vector<std::vector<int>>> examRoomsValidity;
    std::optional<std::vector<std::set<int>>> enrollment; // used to create adjacency matrix 'examCollisions'. could be used to sum exam sizes
    std::optional<std::vector<std::vector<int>>> examsCollisions;
    std::optional<std::vector<std::set<int>>> examCollisionExams;
    std::optional<std::vector<int>> examDegree; //Initial Solution Graph Coloring.
    std::optional<std::vector<std::set<int>>> examSamePeriod;
    std::optional<std::vector<std::vector<std::vector<int>>>> examsCollisionsLimit;
    std::optional<std::vector<std::vector<std::vector<int>>>> examsCollisionsAbove;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATATMP_H

//
// Created by hansk on 10.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTION_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTION_H


#include <vector>
#include <set>
#include <memory>
#include "ExamTTData.h"

struct ExamTTSolution {
    const std::shared_ptr<ExamTTData> examData;

    explicit ExamTTSolution(std::shared_ptr<ExamTTData> examData_ptr) : examData(std::move(examData_ptr)) {
        const auto NUMBER_OF_PERIODS = examData->periodID.size();
        const auto NUMBER_OF_EXAMS = examData->examID.size();
        const auto NUMBER_OF_ROOMS = examData->roomID.size();
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

        periodRoomsAvailability.resize(NUMBER_OF_PERIODS, std::vector<int>(NUMBER_OF_ROOMS, -1));
        for (int room = 0; room < examData->roomPeriodsValidity.size(); ++room)
            for (int period = 0; period < examData->roomPeriodsValidity.at(room).size(); ++period)
                if (examData->roomPeriodsValidity.at(room).at(period) == 1)
                    periodRoomsAvailability.at(period).at(room) = 1;
    }
    //these two essentially represent the timetable. Indexes will have to be replaced with original IDs.
    /**@brief Set of rooms' indexes for each exam*/
    std::vector<std::set<int>> examRooms;
    /**@brief Period's index for each exam*/
    std::vector<int> examPeriod;

    /**@brief 1 if room available, 0 if room assigned, -1 if room invalid*/
    std::vector<std::vector<int>> periodRoomsAvailability; //includes validity. per period more convenient
    /**@brief Weighted matrix showing collision between periods (row) and exams (column).*/
    std::vector<std::vector<int>> periodExamCollisions;
    /** Set of exams scheduled in each period */
    std::vector<std::set<int>> periodExams;
    std::vector<int> examDegreeOfSaturation; //Initial Solution Graph Coloring.
    int cost = 0;
    double runTime = 0;
    std::string creationDateTime;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTION_H

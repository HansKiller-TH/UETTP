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
    ExamTTSolution() = default;

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

    ExamTTSolution(const ExamTTSolution &other) : examData(other.examData) {
        if (this != &other) {
            this->examPeriod = other.examPeriod;
            this->examRooms = other.examRooms;
            this->periodRoomsAvailability = other.periodRoomsAvailability;
            this->periodExamCollisions = other.periodExamCollisions;
            this->periodExams = other.periodExams;
            this->creationDateTime = other.creationDateTime;
            this->costAndTimeLog = other.costAndTimeLog;
            this->configuration = other.configuration;
        }
    }

    ExamTTSolution &operator=(const ExamTTSolution &other) {
        if (this != &other) {
            this->examPeriod = other.examPeriod;
            this->examRooms = other.examRooms;
            this->periodRoomsAvailability = other.periodRoomsAvailability;
            this->periodExamCollisions = other.periodExamCollisions;
            this->periodExams = other.periodExams;
            this->creationDateTime = other.creationDateTime;
            this->costAndTimeLog = other.costAndTimeLog;
            this->configuration = other.configuration;
        }
        return *this;
    }

    bool operator==(const ExamTTSolution &other) const {
        return *examData == *(other.examData) &&
               examPeriod == other.examPeriod &&
               examRooms == other.examRooms &&
               periodRoomsAvailability == other.periodRoomsAvailability &&
               periodExamCollisions == other.periodExamCollisions &&
               periodExams == other.periodExams &&
               creationDateTime == other.creationDateTime &&
               costAndTimeLog == other.costAndTimeLog &&
               configuration == other.configuration;
    }

    bool operator!=(const ExamTTSolution &other) const {
        return !(*this == other);
    }

    /**@brief also creates a copy of the examData obj */
    [[nodiscard]] ExamTTSolution deepCopy() const {
        return {*this, true};
    }

    const std::shared_ptr<ExamTTData> examData;

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
    int costLimit = 0;
    int costAbove = 0;
    std::string creationDateTime;
    std::vector<std::pair<int, double>> costAndTimeLog;
    std::string configuration;
private:
    ExamTTSolution(const ExamTTSolution &other, bool deepCopy)
            : examData(std::make_shared<ExamTTData>(*other.examData)) {
        this->examPeriod = other.examPeriod;
        this->examRooms = other.examRooms;
        this->periodRoomsAvailability = other.periodRoomsAvailability;
        this->periodExamCollisions = other.periodExamCollisions;
        this->periodExams = other.periodExams;
        this->creationDateTime = other.creationDateTime;
        this->costAndTimeLog = other.costAndTimeLog;
        this->configuration = other.configuration;
    }
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTION_H

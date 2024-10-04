//
// Created by hansk on 04.06.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATABUILDER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATABUILDER_H


#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <optional>
#include "ExamTTData.h"
#include "ExamTTDataTMP.h"

class ExamTTDataBuilder {
public:
    explicit ExamTTDataBuilder(const std::string& filepath) : tmp() {
        tmp.filePath = filepath;
    }
    ExamTTDataBuilder processPeriods(const std::vector<std::vector<std::string>> &periods);
    ExamTTDataBuilder processRooms(const std::vector<std::vector<std::string>> &rooms);
    ExamTTDataBuilder processRoomsValidPeriods(const std::vector<std::vector<std::string>> &roomsPeriods);
    ExamTTDataBuilder processExams(const std::vector<std::vector<std::string>> &exams);
    ExamTTDataBuilder processExamValidPeriods(const std::vector<std::vector<std::string>> &examsPeriods);
    ExamTTDataBuilder processExamsValidRooms(const std::vector<std::vector<std::string>> &examsRooms);
    ExamTTDataBuilder processStudentsExams(const std::vector<std::vector<std::string>> &studentsExams);
    ExamTTDataBuilder processSamePeriods(const std::vector<std::vector<std::string>> &exams);
    ExamTTDataBuilder createCollisionsFromEnrollment();
    ExamTTDataBuilder createAllPossibleRoomCombinationsForEachExam();

    void createCollisionMatrixLimitEnrolment();

    /*void setUpSolutionDependentData();

    void processSolutionExamPeriod(const std::vector<std::vector<std::string>> &examPeriod);
    void processSolutionExamRooms(const std::vector<std::vector<std::string>> &examRooms);*/

    std::shared_ptr<ExamTTData> build();
    ExamTTDataTMP tmp;
private:
    /*std::optional<std::string> filePath;
    std::optional<std::string> timeStamp;
    std::optional<std::string> dataSet;
    std::optional<std::vector<int>> periodID;
    *//**@brief The value at each index represents the 1-based day of that period.*//*
    std::optional<std::vector<int>> periodDay;
    std::optional<std::vector<int>> periodSlot;
    std::optional<std::vector<int>> periodWeek;
    std::optional<std::vector<std::string>> periodDate;
    std::optional<std::vector <int>> roomID; //use id when writing timetable solution_ to file
    std::optional<std::vector <std::string>> roomName; //for convenience only
    std::optional<std::vector <int>> roomSize;
    std::optional<std::vector<RoomType>> roomType;
    *//**@brief Binary matrix: for room (row) is period (column) valid.
    * Possible periodDay for each room. Does not say if room is actually available in period.*//*
    std::optional<std::vector<std::vector<int>>> roomPeriodsValidity; //inconvenient. not used currently
    std::optional<std::vector<int>> examID; //use id when writing timetable solution_ to file
    std::optional<std::vector<std::string>> examName; //for convenience only
    std::optional<std::vector<int>> examSize;
    *//**@brief Binary matrix: for exam (row) is period (column) valid?
     *
     * Possible period for each exam. Does not say if period can actually be assigned to exam.*//*
    std::optional<std::vector<std::vector<int>>> examPeriodsValidity;
    *//**@brief Binary matrix: for exam (row) is room (column) valid.
     * Possible rooms for each exam. Does not say if room can actually be assigned to exam.*//*
    std::optional<std::vector<std::vector<int>>> examRoomsValidity;
    std::optional<std::vector<std::set<int>>> enrollment; // used to create adjacency matrix 'examCollisions'. could be used to sum exam sizes
    *//**@brief Weighted adjacency matrix.
     *
     * Collision between exams and number of shared students. No collision == 0. Collision > 0. For exam with itself value is -1.*//*
    std::optional<std::vector<std::vector<int>>> examsCollisions;
    *//**@brief Weighted adjacency matrices for students enrolled for up to a number of exams inclusive.
     *
     * Collision between exams and number of shared students. No collision == 0. Collision > 0. For exam with itself value is -1.*//*
    std::optional<std::vector<std::vector<std::vector<int>>>> examsCollisionsLimit;
    *//**@brief Weighted adjacency matrices for students enrolled for more than a number of exams.
     *
     * Collision between exams and number of shared students. No collision == 0. Collision > 0. For exam with itself value is -1.*//*
    std::optional<std::vector<std::vector<std::vector<int>>>> examsCollisionsAbove;
    *//**@brief Adjacency list
     *
     * For each exam the set of exams it has collision with. Set may be empty*//*
    std::optional<std::vector<std::set<int>>> examCollisionExams;
    std::optional<std::vector<int>> examDegree; //Initial Solution Graph Coloring.
    *//**@brief For each exam the exams that have to be in the same period with it
     * if any*//*
    std::optional<std::vector<std::set<int>>> examSamePeriod;*/
    static RoomType stringToRoomType(const std::string &str);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATABUILDER_H

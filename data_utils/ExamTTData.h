//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATA_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATA_H

#include <string>
#include <ctime>
#include <chrono>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <filesystem>
#include "DateTimeProvider.h"

/**@brief Holds the data to create or change a solution_ */
struct ExamTTData{
    enum class RoomType {Online, Single, Normal, External, Unknown};
    std::string filePath;
    std::string timeStamp;
    std::string dataSet;
    void setMeta(std::string filepath){
        this->filePath = std::move(filepath);
        this->timeStamp = DateTimeProvider::getDateTimeNow();
        // just the filename without extension
        this->dataSet = std::filesystem::path(this->filePath).stem().string();
    }
    //These data structures have to be filled from a file
    std::vector<int> periodID;
    /**@brief The value at each index represents the 1-based day of that period.*/
    std::vector<int> periodDay;
    std::vector<int> periodSlot;
    std::vector<int> periodWeek;
    std::vector<std::string> periodDate;
    std::vector <int> roomID; //use id when writing timetable solution_ to file
    std::vector <std::string> roomName; //for convenience only
    std::vector <int> roomSize;
    std::vector<RoomType> roomType;
    /**@brief Binary matrix: for room (row) is period (column) valid.
    * Possible periodDay for each room. Does not say if room is actually available in period.*/
    std::vector<std::vector<int>> roomPeriodsValidity; //inconvenient. not used currently
    std::vector<int> examID; //use id when writing timetable solution_ to file
    std::vector<std::string> examName; //for convenience only
    std::vector<int> examSize;
    /**@brief Binary matrix: for exam (row) is period (column) valid?
     *
     * Possible period for each exam. Does not say if period can actually be assigned to exam.*/
    std::vector<std::vector<int>> examPeriodsValidity;
    /**@brief Binary matrix: for exam (row) is room (column) valid.
     * Possible rooms for each exam. Does not say if room can actually be assigned to exam.*/
    std::vector<std::vector<int>> examRoomsValidity;
    std::vector<std::set<int>> enrollment; // used to create adjacency matrix 'examCollisions'. could be used to sum exam sizes
    /**@brief Weighted adjacency matrix.
     *
     * Collision between exams and number of shared students. No collision == 0. Collision > 0. For exam with itself value is -1.*/
    std::vector<std::vector<int>> examsCollisions;
    /**@brief Adjacency list
     *
     * For each exam the set of exams it has collision with. Set may be empty*/
    std::vector<std::set<int>> examCollisionExams;
    std::vector<int> examDegree; //Initial Solution Graph Coloring.
    /**@brief For each exam the exams that have to be in the same period with it
     * if any*/
    std::vector<std::set<int>> examSamePeriod;
//private:
    // Solution dependent data
    //these two essentially represent the timetable. Indexes will have te be replaced with original IDs.
    /**@brief List of rooms' indexes for each exam*/
    std::vector<std::set<int>> examRooms;
    /**@brief Period's index for each exam*/
    std::vector<int> examPeriod;
    /**@brief 1 if room available, 0 if room assigned, -1 if room invalid*/
    std::vector<std::vector<int>> periodRoomsAvailability; //includes validity. per period more convenient
    /**@brief Weighted matrix showing collision between periods (row) and exams (column).
     * Used to determine Degree Of Saturation for Graph Coloring initial solution_.*/
    std::vector<std::vector<int>> periodExamCollisions;
    std::vector<std::set<int>> periodExams;
    std::vector<int> examDegreeOfSaturation; //Initial Solution Graph Coloring.

    int cost = 0;
    double runTime = 0;
    int costLimit = 0;
    int costAbove = 0;
    std::string creationDateTime;
    /** cost and time */
    std::vector<std::pair<int,double>> costAndTimeLog;
    std::string configuration;

    /** Collision matrix for student with 7 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit7;
    /** Collision matrix for student with more than 7 enrolments */
    std::vector<std::vector<int>> examsCollisionsBeyond7;
    /** Collision matrix for student with 8 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit8;
    /** Collision matrix for student with 9 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit9;
    /** Collision matrix for student with 10 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit10;
    /** Collision matrix for student with 11 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit11;
    /** Collision matrix for student with 12 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit12;
    /** Collision matrix for student with 13 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit13;
    /** Collision matrix for student with 14 enrolments max */
    std::vector<std::vector<int>> examsCollisionsLimit14;
};
#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATA_H

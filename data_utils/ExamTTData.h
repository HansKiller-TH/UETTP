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
#include "ExamTTDataTMP.h"
//#include "ExamTTDataBuilder.h"
#include "RoomType.h"

/**@brief Holds the data to create or change a solution_ */
struct ExamTTData {
    explicit ExamTTData(ExamTTDataTMP tmp) : filePath(std::move(tmp.filePath.value())),
                                                timeStamp(DateTimeProvider::getDateTimeNow()),
                                                dataSet(std::filesystem::path(this->filePath).stem().string()),
                                                periodID(tmp.periodID.value()),
                                                periodDay(tmp.periodDay.value()),
                                                periodSlot(tmp.periodSlot.value()),
                                                periodWeek(tmp.periodWeek.value()),
                                                periodDate(tmp.periodDate.value()),
                                                roomID(tmp.roomID.value()),
                                                roomName(tmp.roomName.value()),
                                                roomSize(tmp.roomSize.value()),
                                                roomType(tmp.roomType.value()),
                                                roomPeriodsValidity(tmp.roomPeriodsValidity.value()),
                                                examID(tmp.examID.value()),
                                                examName(tmp.examName.value()),
                                                examSize(tmp.examSize.value()),
                                                examPeriodsValidity(tmp.examPeriodsValidity.value()),
                                                examRoomsValidity(tmp.examRoomsValidity.value()),
                                                enrollment(tmp.enrollment.value()),
                                                examsCollisions(tmp.examsCollisions.value()),
                                                examsCollisionsLimit(tmp.examsCollisionsLimit.value()),
                                                examsCollisionsAbove(tmp.examsCollisionsAbove.value()),
                                                examCollisionExams(tmp.examCollisionExams.value()),
                                                examDegree(tmp.examDegree.value()),
                                                examSamePeriod(tmp.examSamePeriod.value()){}

    const std::string filePath;
    const std::string timeStamp;
    const std::string dataSet;
    //These data structures have to be filled from a file
    const std::vector<int> periodID;
    /**@brief The value at each index represents the 1-based day of that period.*/
    const std::vector<int> periodDay;
    const std::vector<int> periodSlot;
    const std::vector<int> periodWeek;
    const std::vector<std::string> periodDate;
    const std::vector<int> roomID; //use id when writing timetable solution_ to file
    const std::vector<std::string> roomName; //for convenience only
    const std::vector<int> roomSize;
    const std::vector<RoomType> roomType;
    /**@brief Binary matrix: for room (row) is period (column) valid.
    * Possible periodDay for each room. Does not say if room is actually available in period.*/
    const std::vector<std::vector<int>> roomPeriodsValidity; //inconvenient. not used currently
    const std::vector<int> examID; //use id when writing timetable solution_ to file
    const std::vector<std::string> examName; //for convenience only
    const std::vector<int> examSize;
    /**@brief Binary matrix: for exam (row) is period (column) valid?
     *
     * Possible period for each exam. Does not say if period can actually be assigned to exam.*/
    const std::vector<std::vector<int>> examPeriodsValidity;
    /**@brief Binary matrix: for exam (row) is room (column) valid.
     * Possible rooms for each exam. Does not say if room can actually be assigned to exam.*/
    const std::vector<std::vector<int>> examRoomsValidity;
    const std::vector<std::set<int>> enrollment; // used to create adjacency matrix 'examCollisions'. could be used to sum exam sizes
    /**@brief Weighted adjacency matrix.
     *
     * Collision between exams and number of shared students. No collision == 0. Collision > 0. For exam with itself value is -1.*/
    const std::vector<std::vector<int>> examsCollisions;
    /**@brief Adjacency list
     *
     * For each exam the set of exams it has collision with. Set may be empty*/
    const std::vector<std::set<int>> examCollisionExams;
    const std::vector<int> examDegree; //Initial Solution Graph Coloring.
    /**@brief For each exam the exams that have to be in the same period with it
     * if any*/
    const std::vector<std::set<int>> examSamePeriod;
    const std::vector<std::vector<std::vector<int>>> examsCollisionsLimit;
    const std::vector<std::vector<std::vector<int>>> examsCollisionsAbove;
    bool operator==(const ExamTTData& other) const {
        return filePath == other.filePath &&
               timeStamp == other.timeStamp &&
               dataSet == other.dataSet &&
               periodID == other.periodID &&
               periodDay == other.periodDay &&
               periodSlot == other.periodSlot &&
               periodWeek == other.periodWeek &&
               periodDate == other.periodDate &&
               roomID == other.roomID &&
               roomName == other.roomName &&
               roomSize == other.roomSize &&
               roomType == other.roomType &&
               roomPeriodsValidity == other.roomPeriodsValidity &&
               examID == other.examID &&
               examName == other.examName &&
               examSize == other.examSize &&
               examPeriodsValidity == other.examPeriodsValidity &&
               examRoomsValidity == other.examRoomsValidity &&
               enrollment == other.enrollment &&
               examsCollisions == other.examsCollisions &&
               examsCollisionsLimit == other.examsCollisionsLimit &&
               examsCollisionsAbove == other.examsCollisionsAbove &&
               examCollisionExams == other.examCollisionExams &&
               examDegree == other.examDegree &&
               examSamePeriod == other.examSamePeriod;
    }
};

#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATA_H

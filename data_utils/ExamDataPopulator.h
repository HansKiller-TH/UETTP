//
// Created by hansk on 04.06.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAPOPULATOR_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAPOPULATOR_H


#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "ExamTTData.h"

class ExamDataPopulator {
public:
    explicit ExamDataPopulator(std::shared_ptr<ExamTTData> examData) : examData(std::move(examData)){};
    void processPeriods(const std::vector<std::vector<std::string>> &periods);
    void processRooms(const std::vector<std::vector<std::string>> &rooms);
    void processRoomsValidPeriods(const std::vector<std::vector<std::string>> &roomsPeriods);
    void processExams(const std::vector<std::vector<std::string>> &exams);
    void processExamValidPeriods(const std::vector<std::vector<std::string>> &examsPeriods);
    void processExamsValidRooms(const std::vector<std::vector<std::string>> &examsRooms);
    void processStudentsExams(const std::vector<std::vector<std::string>> &studentsExams);
    void processSamePeriods(const std::vector<std::vector<std::string>> &exams);

    void createCollisionsFromEnrollment();
    void createCollisionMatrixLimitEnrolment();

    void setUpSolutionDependentData();

    void processSolutionExamPeriod(const std::vector<std::vector<std::string>> &examPeriod);
    void processSolutionExamRooms(const std::vector<std::vector<std::string>> &examRooms);
private:
    std::shared_ptr<ExamTTData> examData;
    static ExamTTData::RoomType stringToRoomType(const std::string &str);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAPOPULATOR_H

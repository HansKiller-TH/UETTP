#include "gtest/gtest.h"
#include "Algorithm/ExamTTSolutionManipulator.h"
#include "data_utils/ExamTTSolution.h"
#include "data_utils/ExamTTDataBuilder.h"

//
// Created by hansk on 17.05.2024.
//
class ExamDataManipulatorTest : public ::testing::Test {
protected:
    ExamTTSolutionManipulator manipulator; // Instance of your class to test

    // You can add setup code here, it will be executed before each test
    void SetUp() override {
    }
};

TEST_F(ExamDataManipulatorTest, setDataTest) {
    ExamTTDataTMP tmp;
    tmp.examID.value() = {1};
    auto examTTData = std::make_shared<ExamTTData>(tmp);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    auto old = manipulator.getSolution();
    ASSERT_EQ(manipulator.getSolution(), old);
}

TEST_F(ExamDataManipulatorTest, getRandomExamTest) {
    ExamTTDataTMP tmp;
    tmp.examID.value() = {1};
    auto examTTData = std::make_shared<ExamTTData>(tmp);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    auto result = manipulator.getRandomExam();
    ASSERT_EQ(result, 0);
}

TEST_F(ExamDataManipulatorTest, getRandomExamWithConnectedTest) {
    ExamTTDataTMP tmp;
    tmp.examID.value() = {1}; // The values are never used. Only the indexes.
    tmp.examSamePeriod.value() = {{2, 3}};
    auto examTTData = std::make_shared<ExamTTData>(tmp);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    auto result = manipulator.getRandomExamWithConnected();
    std::set<int> asserter = {0, 2, 3};
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getRandomExamWithConnectedWithPeriodTest) {
    ExamTTDataTMP tmp;
    tmp.examID.value() = {1}; // The values are never used. Only the indexes.
    tmp.examSamePeriod.value() = {{2, 3}};
    auto examTTData = std::make_shared<ExamTTData>(tmp);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->examPeriod = {5};
    manipulator.setSolution(solution);
    auto result = manipulator.getRandomExamWithConnectedWithPeriod();
    std::pair<std::set<int>, int> asserter = {{0, 2, 3}, 5};
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, trySwitchUsedRoomsTest) {
    ExamTTDataTMP examDataTMP;
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->periodRoomsAvailability = {{0, 1, -1, 1,  0, -1, 1},
                                         {1, 0, 1,  -1, 0, -1, 1}};
    manipulator.setSolution(solution);
    std::vector<std::vector<int>> asserter = {{1, 0, -1, 1,  0, -1, 1},
                                              {0, 1, 1,  -1, 0, -1, 1}};
    manipulator.trySwitchUsedRooms(PeriodChange(0), PeriodChange(1));
    ASSERT_EQ(manipulator.getSolution()->periodRoomsAvailability, asserter);
}

TEST_F(ExamDataManipulatorTest, moveExamToPeriodTest) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.examCollisionExams.value() = {{}};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->examPeriod = {1};
    solution->periodExams = {{},
                             {1, 0},
                             {3}};
    manipulator.setSolution(solution);
    int period = 2;
    std::vector<std::set<int>> asserterPeriodExams = {{},
                                                      {1},
                                                      {0, 3}};
    manipulator.moveExamToPeriod(0, period);
    ASSERT_EQ(manipulator.getSolution()->examPeriod.at(0), period);
    ASSERT_EQ(manipulator.getSolution()->periodExams, asserterPeriodExams);
}

TEST_F(ExamDataManipulatorTest, moveExamToPeriod_Unscheduled_Test) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.examCollisionExams.value() = {{}};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->examPeriod = {-1};
    solution->periodExams = {{1},
                             {2},
                             {3}};
    manipulator.setSolution(solution);
    int period = 2;
    std::vector<std::set<int>> asserterPeriodExams = {{1},
                                                      {2},
                                                      {0, 3}};
    manipulator.moveExamToPeriod(0, period);
    ASSERT_EQ(manipulator.getSolution()->examPeriod.at(0), period);
    ASSERT_EQ(manipulator.getSolution()->periodExams, asserterPeriodExams);
}

TEST_F(ExamDataManipulatorTest, moveExamToPeriod_Deschedule_Test) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.examCollisionExams.value() = {{}};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->examPeriod = {1};
    solution->periodExams = {{1},
                             {0, 2},
                             {3}};
    manipulator.setSolution(solution);
    int period = -1;
    std::vector<std::set<int>> asserterPeriodExams = {{1},
                                                      {2},
                                                      {3}};
    manipulator.moveExamToPeriod(0, period);
    ASSERT_EQ(manipulator.getSolution()->examPeriod.at(0), period);
    ASSERT_EQ(manipulator.getSolution()->periodExams, asserterPeriodExams);
}

TEST_F(ExamDataManipulatorTest, kempeChainTest) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.examSamePeriod.value() = {{},
                                          {},
                                          {},
                                          {4},
                                          {3},
                                          {},
                                          {7},
                                          {6},
                                          {}};
    examDataTMP.examsCollisions.value() = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                                           {0, 0, 0, 0, 0, 0, 1, 0, 0},
                                           {0, 0, 0, 1, 0, 0, 0, 0, 0},
                                           {0, 0, 1, 0, 0, 0, 0, 0, 0},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                           {0, 1, 0, 0, 0, 0, 0, 0, 0},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 0}};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    std::set<int> df = {1};
    std::set<int> ds = {2};
    std::set<int> ef = {3, 4, 5};
    std::set<int> es = {6, 7, 8};
    std::set<int> asserterDf = {1, 3, 4};
    std::set<int> asserterDs = {2, 6, 7};
    std::set<int> asserterEf = {5};
    std::set<int> asserterEs = {8};
    manipulator.kempeChain(df, ds, ef, es);
    ASSERT_EQ(df, asserterDf);
    ASSERT_EQ(ds, asserterDs);
    ASSERT_EQ(ef, asserterEf);
    ASSERT_EQ(es, asserterEs);
}

TEST_F(ExamDataManipulatorTest, getAllExamsTest) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.examID.value() = {12, 45, 100, 3, 2};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    std::set<int> asserter = {0, 1, 2, 3, 4};
    auto result = manipulator.getAllExams();
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNextPeriodSameDayTest) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.periodDay.value() = {4, 5, 5, 5, 6};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    auto result = manipulator.getNextPeriodSameDay(1);
    int asserter = 2;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNextPeriodSameDay_NextPeriodDifferentDay_Test) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.periodDay.value() = {4, 5, 5, 5, 6};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    auto result = manipulator.getNextPeriodSameDay(3);
    int asserter = -1;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNextPeriodSameDay_NoNextPeriod_Test) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.periodDay.value() = {4, 5, 5, 5, 6};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    auto result = manipulator.getNextPeriodSameDay(4);
    int asserter = -1;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriodTest) {
    ExamTTDataTMP examDataTMP;
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    manipulator.setSolution(solution);
    std::set<int> exams = {0, 2, 4, 5};
    ASSERT_TRUE(manipulator.hasAnyExamCollisionWithAnyPeriod(exams, {0}));
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriod_All_Test) {
    ExamTTDataTMP examDataTMP;
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    manipulator.setSolution(solution);
    std::set<int> exams = {1, 3, 5};
    ASSERT_TRUE(manipulator.hasAnyExamCollisionWithAnyPeriod(exams, {0}));
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriod_None_Test) {
    ExamTTDataTMP examDataTMP;
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    manipulator.setSolution(solution);
    std::set<int> exams = {0, 2, 4};
    ASSERT_FALSE(manipulator.hasAnyExamCollisionWithAnyPeriod(exams, {0}));
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriod_NonePeriod_Test) {
    ExamTTDataTMP examDataTMP;
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    solution->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    manipulator.setSolution(solution);
    std::set<int> exams = {0, 2, 4};
    ASSERT_FALSE(manipulator.hasAnyExamCollisionWithAnyPeriod(exams, {-1}));
}

TEST_F(ExamDataManipulatorTest, getValidPeriodsForExamsTest) {
    ExamTTDataTMP examDataTMP;
    examDataTMP.examPeriodsValidity = {{0, 1, 0, 1, 0},
                                       {0, 1, 1, 0, 0},
                                       {0, 1, 0, 0, 0}};
    auto examTTData = std::make_shared<ExamTTData>(examDataTMP);
    auto solution = std::make_shared<ExamTTSolution>(examTTData);
    manipulator.setSolution(solution);
    std::set<int> exams = {0, 1, 2};
    auto result = manipulator.getValidPeriodsForExams(exams);
    std::set<int> asserter = {1};
    ASSERT_EQ(result, asserter);
}
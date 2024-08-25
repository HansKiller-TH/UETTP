#include <gtest/gtest.h>
#include "Algorithm/ExamTTSolutionManipulator.h"

//
// Created by hansk on 17.05.2024.
//
class ExamDataManipulatorTest : public ::testing::Test {
protected:
    std::shared_ptr<ExamTTData> examData = std::make_shared<ExamTTData>();
    ExamDataManipulator manipulator = ExamDataManipulator(examData); // Instance of your class to test

    // You can add setup code here, it will be executed before each test
    void SetUp() override {
    }
};

TEST_F(ExamDataManipulatorTest,setDataTest){
    auto tmp = manipulator.getData();
    auto asserter = std::make_shared<ExamTTData>();
    examData->examPeriod={1,2,3,4,5};
    manipulator.setData(asserter);
    ASSERT_EQ(manipulator.getData(),asserter);
    ASSERT_NE(manipulator.getData(),tmp);
}

TEST_F(ExamDataManipulatorTest, getRandomExamTest) {
    examData->examID = {1};
    auto result = manipulator.getRandomExam();
    ASSERT_EQ(result, 0);
}

TEST_F(ExamDataManipulatorTest, getRandomExamWithConnectedTest) {
    examData->examID = {1}; // The values are never used. Only the indexes.
    examData->examSamePeriod = {{2, 3}};
    auto result = manipulator.getRandomExamWithConnected();
    std::set<int> asserter = {0, 2, 3};
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getRandomExamWithConnectedWithPeriodTest) {
    examData->examID = {1}; // The values are never used. Only the indexes.
    examData->examSamePeriod = {{2, 3}};
    examData->examPeriod = {5};
    auto result = manipulator.getRandomExamWithConnectedWithPeriod();
    std::pair<std::set<int>, int> asserter = {{0, 2, 3}, 5};
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getRandomRoomsForExamInPeriodTest) {
    examData->examSize = {11};
    examData->roomSize = {4, 5, 7, 20, 50};
    examData->periodRoomsAvailability = {{1, 1, 1, 0, -1}};
    examData->examRoomsValidity = {{1, 1, 1, 0, 1}};
    std::set<int> asserterOne = {0, 2};
    std::set<int> asserterTwo = {1, 2};
    auto result = manipulator.getRandomRoomsForExamInPeriod(0, 0);
    ASSERT_TRUE(result == asserterOne || result == asserterTwo);
}

TEST_F(ExamDataManipulatorTest, getBestFittingRoomsForExamTest) {
    examData->examSize = {11};
    examData->roomSize = {4, 5, 6, 7, 50};
    examData->periodRoomsAvailability = {{1, 1, 1, 1, -1}};
    examData->examRoomsValidity = {{1, 1, 1, 1, 1}};
    std::set<int> asserter = {1, 2};
    auto result = manipulator.getBestFittingRoomsForExam({0,11}, {1, 1, 1, 1, -1});
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, reassignRoomsToExamSamePeriodTest) {
    examData->examPeriod = {0};
    examData->examRooms = {{0, 1, 2}};
    examData->periodRoomsAvailability = {{0, 0, 0, 1, 1, 1, -1}};
    std::set<int> newRooms = {3, 4, 5};
    manipulator.reassignRoomsToExamSamePeriod(0, newRooms);
    std::vector<std::vector<int>> asserter = {{1, 1, 1, 0, 0, 0, -1}};
    ASSERT_EQ(examData->periodRoomsAvailability, asserter);
}

TEST_F(ExamDataManipulatorTest, trySwitchUsedRoomsTest) {
    examData->periodRoomsAvailability = {{0, 1, -1, 1,  0, -1, 1},
                                         {1, 0, 1,  -1, 0, -1, 1}};
    std::vector<std::vector<int>> asserter = {{1, 0, -1, 1,  0, -1, 1},
                                              {0, 1, 1,  -1, 0, -1, 1}};
    manipulator.trySwitchUsedRooms(0, 1);
    ASSERT_EQ(examData->periodRoomsAvailability, asserter);
}

TEST_F(ExamDataManipulatorTest, tryAssignBestFittingRoomsForEachExamInPeriodWithoutTest) {
    std::set<int> exams = {1, 2, 3};
    int period = 0;
    std::set<int> without = {4, 5};
    examData->periodRoomsAvailability = {{-1, 1, 1, 1, 0, 0}};
    examData->roomSize = {5, 6, 7, 8, 9, 12};
    examData->examSize = {4, 5, 6, 7, 8, 9, 10};
    examData->examRooms = {{},
                           {2},
                           {3},
                           {4},
                           {4},
                           {5},
                           {}};
    examData->examRoomsValidity = {{1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1}};
    ASSERT_TRUE(manipulator.tryAssignBestFittingRoomsForEachExamInPeriodWithout(exams, period, without));
    std::vector<std::vector<int>> asserterPeriodRoomsAvailability = {{-1, 0, 0, 0, 1, 1}};
    std::vector<std::set<int>> asserterExamRooms = {{},
                                                    {3},
                                                    {1},
                                                    {2},
                                                    {4},
                                                    {5},
                                                    {}};
    ASSERT_EQ(examData->periodRoomsAvailability, asserterPeriodRoomsAvailability);
    ASSERT_EQ(examData->examRooms, asserterExamRooms);
}
TEST_F(ExamDataManipulatorTest, tryAssignBestFittingRoomsForEachExamInPeriodWithout_NotEnoughRoom_Test) {
    std::set<int> exams = {1, 2, 3};
    int period = 0;
    std::set<int> without = {4, 5};
    examData->periodRoomsAvailability = {{-1, -1, -1, -1, 0, 0}};
    examData->roomSize = {5, 6, 7, 8, 9, 12};
    examData->examSize = {4, 5, 6, 7, 8, 9, 10};
    examData->examRooms = {{},
                           {2},
                           {3},
                           {4},
                           {4},
                           {5},
                           {}};
    examData->examRoomsValidity = {{1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1},
                                   {1, 1, 1, 1, 1, 1}};
    ASSERT_FALSE(manipulator.tryAssignBestFittingRoomsForEachExamInPeriodWithout(exams, period, without));
}

TEST_F(ExamDataManipulatorTest, moveExamToPeriodTest) {
    examData->examPeriod = {1};
    examData->periodExams = {{},
                             {1, 0},
                             {3}};
    examData->examCollisionExams = {{}};
    int period = 2;
    manipulator.moveExamToPeriod(0, period);
    ASSERT_EQ(examData->examPeriod.at(0), period);
    std::vector<std::set<int>> asserterPeriodExams = {{},
                                                      {1},
                                                      {0, 3}};
    ASSERT_EQ(examData->periodExams, asserterPeriodExams);
}

TEST_F(ExamDataManipulatorTest, moveExamToPeriod_Unscheduled_Test) {
    examData->examPeriod = {-1};
    examData->periodExams = {{1},
                             {2},
                             {3}};
    examData->examCollisionExams = {{}};
    int period = 2;
    manipulator.moveExamToPeriod(0, period);
    ASSERT_EQ(examData->examPeriod.at(0), period);
    std::vector<std::set<int>> asserterPeriodExams = {{1},
                                                      {2},
                                                      {0, 3}};
    ASSERT_EQ(examData->periodExams, asserterPeriodExams);
}

TEST_F(ExamDataManipulatorTest, moveExamToPeriod_Deschedule_Test) {
    examData->examPeriod = {1};
    examData->periodExams = {{1},
                             {0, 2},
                             {3}};
    examData->examCollisionExams = {{}};
    int period = -1;
    manipulator.moveExamToPeriod(0, period);
    ASSERT_EQ(examData->examPeriod.at(0), period);
    std::vector<std::set<int>> asserterPeriodExams = {{1},
                                                      {2},
                                                      {3}};
    ASSERT_EQ(examData->periodExams, asserterPeriodExams);
}

TEST_F(ExamDataManipulatorTest, kempeChainTest) {
    std::set<int> df = {1};
    std::set<int> ds = {2};
    std::set<int> ef = {3, 4, 5};
    std::set<int> es = {6, 7, 8};
    examData->examSamePeriod = {{},
                                {},
                                {},
                                {4},
                                {3},
                                {},
                                {7},
                                {6},
                                {}};
    examData->examsCollisions = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 1, 0, 0},
                                 {0, 0, 0, 1, 0, 0, 0, 0, 0},
                                 {0, 0, 1, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 1, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0}};
    manipulator.kempeChain(df, ds, ef, es);
    std::set<int> asserterDf = {1, 3, 4};
    std::set<int> asserterDs = {2, 6, 7};
    std::set<int> asserterEf = {5};
    std::set<int> asserterEs = {8};
    ASSERT_EQ(df, asserterDf);
    ASSERT_EQ(ds, asserterDs);
    ASSERT_EQ(ef, asserterEf);
    ASSERT_EQ(es, asserterEs);
}

TEST_F(ExamDataManipulatorTest, getAllExamsTest) {
    examData->examID = {12, 45, 100, 3, 2};
    auto result = manipulator.getAllExams();
    std::set<int> asserter = {0, 1, 2, 3, 4};
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNumberOfPeriodsTest) {
    examData->periodID = {12, 45, 100, 3, 2};
    auto result = manipulator.getNumberOfPeriods();
    int asserter = 5;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNextPeriodSameDayTest) {
    examData->periodDay = {4, 5, 5, 5, 6};
    auto result = manipulator.getNextPeriodSameDay(1);
    int asserter = 2;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNextPeriodSameDay_NextPeriodDifferentDay_Test) {
    examData->periodDay = {4, 5, 5, 5, 6};
    auto result = manipulator.getNextPeriodSameDay(3);
    int asserter = -1;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, getNextPeriodSameDay_NoNextPeriod_Test) {
    examData->periodDay = {4, 5, 5, 5, 6};
    auto result = manipulator.getNextPeriodSameDay(4);
    int asserter = -1;
    ASSERT_EQ(result, asserter);
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriodTest) {
    examData->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    std::set<int> exams = {0, 2, 4, 5};
    ASSERT_TRUE(manipulator.hasAnyExamCollisionWithPeriod(exams, 0));
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriod_All_Test) {
    examData->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    std::set<int> exams = {1, 3, 5};
    ASSERT_TRUE(manipulator.hasAnyExamCollisionWithPeriod(exams, 0));
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriod_None_Test) {
    examData->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    std::set<int> exams = {0, 2, 4};
    ASSERT_FALSE(manipulator.hasAnyExamCollisionWithPeriod(exams, 0));
}

TEST_F(ExamDataManipulatorTest, hasAnyExamCollisionWithPeriod_NonePeriod_Test) {
    examData->periodExamCollisions = {{0, 1, 0, 1, 0, 1}};
    std::set<int> exams = {0, 2, 4};
    ASSERT_FALSE(manipulator.hasAnyExamCollisionWithPeriod(exams, -1));
}

TEST_F(ExamDataManipulatorTest, getValidPeriodsForExamsTest) {
    std::set<int> exams = {0, 1, 2};
    examData->examPeriodsValidity = {{0, 1, 0, 1, 0},
                                     {0, 1, 1, 0, 0},
                                     {0, 1, 0, 0, 0}};
    auto result = manipulator.getValidPeriodsForExams(exams);
    std::set<int> asserter = {1};
    ASSERT_EQ(result, asserter);
}
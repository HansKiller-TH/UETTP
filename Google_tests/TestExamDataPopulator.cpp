//
// Created by hansk on 17.06.2024.
//
#include <gtest/gtest.h>
#include "data_utils/ExamTTData.h"
#include "data_utils/ExamTTDataBuilder.h"

class ExamDataPopulatorTest : public ::testing::Test {
protected:
    std::shared_ptr<ExamTTData> examData = std::make_shared<ExamTTData>();
    ExamDataPopulator pop = ExamDataPopulator(examData); // Instance of your class to test

    // You can add setup code here, it will be executed before each test
    void SetUp() override {
    }
};

TEST_F(ExamDataPopulatorTest, ProcessPeriodsTest) {
    std::vector<std::vector<std::string>> param = {{"1", "1", "7",  "0", "2024-01-18"},
                                                   {"2", "1", "8",  "0", "2024-01-18"},
                                                   {"3", "1", "9",  "0", "2024-01-18"},
                                                   {"4", "1", "10", "0", "2024-01-18"},
                                                   {"5", "2", "7",  "1", "2024-01-19"}};
    pop.processPeriods(param);
    std::vector<int> asserterID = {1, 2, 3, 4, 5};
    ASSERT_EQ(examData->periodID, asserterID);
    std::vector<int> asserterDay = {1, 1, 1, 1, 2};
    ASSERT_EQ(examData->periodDay, asserterDay);
    std::vector<int> asserterSlot = {7, 8, 9, 10, 7};
    ASSERT_EQ(examData->periodSlot, asserterSlot);
    std::vector<int> asserterWeek = {0, 0, 0, 0, 1};
    ASSERT_EQ(examData->periodWeek, asserterWeek);
    std::vector<std::string> asserterDate = {"2024-01-18", "2024-01-18", "2024-01-18", "2024-01-18", "2024-01-19"};
    ASSERT_EQ(examData->periodDate, asserterDate);
}

TEST_F(ExamDataPopulatorTest, ProcessPeriodsVectorSizeTooSmallTest) {
    std::vector<std::vector<std::string>> param = {{"1", "1", "7",  "0", "2024-01-18"},
                                                   {"2", "1", "8",  "2024-01-18"},
                                                   {"3", "1", "9",  "0", "2024-01-18"},
                                                   {"4", "1", "10", "0", "2024-01-18"},
                                                   {"5", "2", "7",  "1", "2024-01-19"}};
    pop.processPeriods(param);
    std::vector<int> asserterID = {1};
    ASSERT_EQ(examData->periodID, asserterID);
    std::vector<int> asserterDay = {1};
    ASSERT_EQ(examData->periodDay, asserterDay);
    std::vector<int> asserterSlot = {7};
    ASSERT_EQ(examData->periodSlot, asserterSlot);
    std::vector<int> asserterWeek = {0};
    ASSERT_EQ(examData->periodWeek, asserterWeek);
    std::vector<std::string> asserterDate = {"2024-01-18"};
    ASSERT_EQ(examData->periodDate, asserterDate);
}

TEST_F(ExamDataPopulatorTest, ProcessRoomsTest) {
    std::vector<std::vector<std::string>> param = {{"1", "A", "7",  "einzeln"},
                                                   {"2", "B", "8",  "online"},
                                                   {"3", "C", "9",  "normal"},
                                                   {"4", "D", "10", "extern"},
                                                   {"5", "E", "11", "1"}};
    pop.processRooms(param);
    std::vector<int> asserterID = {1, 2, 3, 4, 5};
    ASSERT_EQ(examData->roomID, asserterID);
    std::vector<std::string> asserterName = {"A", "B", "C", "D", "E"};
    ASSERT_EQ(examData->roomName, asserterName);
    std::vector<int> asserterSize = {7, 8, 9, 10, 11};
    ASSERT_EQ(examData->roomSize, asserterSize);
    std::vector<ExamTTData::RoomType> asserterType = {ExamTTData::RoomType::Single, ExamTTData::RoomType::Online,
                                                      ExamTTData::RoomType::Normal, ExamTTData::RoomType::External,
                                                      ExamTTData::RoomType::Unknown};
    ASSERT_EQ(examData->roomType, asserterType);
}

TEST_F(ExamDataPopulatorTest, ProcessRoomsVectorSizeTooSmallTest) {
    std::vector<std::vector<std::string>> param = {{"1", "A", "7",  "einzeln"},
                                                   {"2", "B", "8",  "online"},
                                                   {"3", "C", "9"},
                                                   {"4", "D", "10", "extern"},
                                                   {"5", "E", "11", "1"}};
    pop.processRooms(param);
    std::vector<int> asserterID = {1, 2};
    ASSERT_EQ(examData->roomID, asserterID);
    std::vector<std::string> asserterName = {"A", "B"};
    ASSERT_EQ(examData->roomName, asserterName);
    std::vector<int> asserterSize = {7, 8};
    ASSERT_EQ(examData->roomSize, asserterSize);
    std::vector<ExamTTData::RoomType> asserterType = {ExamTTData::RoomType::Single, ExamTTData::RoomType::Online};
    ASSERT_EQ(examData->roomType, asserterType);
}

TEST_F(ExamDataPopulatorTest, ProcessRoomsValidPeriodsTest) {
    examData->roomID = {1, 2, 3};
    examData->periodID = {1, 2, 3, 4, 5};
    std::vector<std::vector<std::string>> param = {{"1", "1"},
                                                   {"1", "2"},
                                                   {"1", "3"},
                                                   {"2", "2"},
                                                   {"2", "3"},
                                                   {"2", "4"},
                                                   {"3", "3"},
                                                   {"3", "4"},
                                                   {"3", "5"}};
    pop.processRoomsValidPeriods(param);
    std::vector<std::vector<int>> asserterPeriodRoomsAvailability = {{1,  -1, -1},
                                                                     {1,  1,  -1},
                                                                     {1,  1,  1},
                                                                     {-1, 1,  1},
                                                                     {-1, -1, 1}};
    ASSERT_EQ(examData->periodRoomsAvailability, asserterPeriodRoomsAvailability);
    std::vector<std::vector<int>> asserterRoomPeriodsValidity = {{1, 1, 1, 0, 0},
                                                                 {0, 1, 1, 1, 0},
                                                                 {0, 0, 1, 1, 1}};
    ASSERT_EQ(examData->roomPeriodsValidity, asserterRoomPeriodsValidity);
}

TEST_F(ExamDataPopulatorTest, ProcessExamsTest) {
    std::vector<std::vector<std::string>> param = {{"1", "A", "7"},
                                                   {"2", "B", "8"},
                                                   {"3", "C", "9"},
                                                   {"4", "D", "10"},
                                                   {"5", "E", "11"}};
    pop.processExams(param);
    std::vector<int> asserterID = {1, 2, 3, 4, 5};
    ASSERT_EQ(examData->examID, asserterID);
    std::vector<std::string> asserterName = {"A", "B", "C", "D", "E"};
    ASSERT_EQ(examData->examName, asserterName);
    std::vector<int> asserterSize = {7, 8, 9, 10, 11};
    ASSERT_EQ(examData->examSize, asserterSize);
}

TEST_F(ExamDataPopulatorTest, ProcessExamsValidPeriodsTest) {
    examData->examID = {1, 2, 3};
    examData->periodID = {1, 2, 3, 4, 5};
    std::vector<std::vector<std::string>> param = {{"1", "1"},
                                                   {"1", "2"},
                                                   {"1", "3"},
                                                   {"2", "2"},
                                                   {"2", "3"},
                                                   {"2", "4"},
                                                   {"3", "3"},
                                                   {"3", "4"},
                                                   {"3", "5"}};
    pop.processExamValidPeriods(param);
    std::vector<std::vector<int>> asserterExamPeriodsValidity = {{1, 1, 1, 0, 0},
                                                                 {0, 1, 1, 1, 0},
                                                                 {0, 0, 1, 1, 1}};
    ASSERT_EQ(examData->examPeriodsValidity, asserterExamPeriodsValidity);
}

TEST_F(ExamDataPopulatorTest, ProcessExamsValidRoomsTest) {
    examData->examID = {1, 2, 3, 4, 5};
    examData->roomID = {1, 2, 3};
    std::vector<std::vector<std::string>> param = {{"1", "1"},
                                                   {"1", "2"},
                                                   {"2", "1"},
                                                   {"2", "2"},
                                                   {"3", "3"},
                                                   {"4", "3"},
                                                   {"5", "1"},
                                                   {"5", "2"},
                                                   {"5", "3"}};
    pop.processExamsValidRooms(param);
    std::vector<std::vector<int>> asserterExamRoomsValidity = {{1, 1, 0},
                                                               {1, 1, 0},
                                                               {0, 0, 1},
                                                               {0, 0, 1},
                                                               {1, 1, 1}};
    ASSERT_EQ(examData->examRoomsValidity, asserterExamRoomsValidity);
}

TEST_F(ExamDataPopulatorTest, ProcessStudentsExamsTest) {
    examData->examID = {
             8531,  9099,  9101,  9132, 10408,  //4
            10409, 10415, 10417, 10418, 10420,  //9
            10421, 10429, 10430, 10432, 10435, //14
            10439, 10440, 10444, 10447, 10448, //19
            10450, 10454, 10468, 10469, 10475, //24
            10479, 10484, 10487, 10488, 10495, //29
            10499, 10500, 10502, 10503, 10504, //34
            10509, 10512, 10519, 10524, 10525, //39
            10526, 10533, 10536, 10539, 10541, //44
            11129, 11138, 11139, 11186, 12801, //49
            12802, 12810, 12813, 12850, 12852, //54
            12853, 13102};
    std::vector<std::vector<std::string>> studentsExams = {{"2811", "10408"},
                                                           {"2813", "10541"},
                                                           {"2813", "10504"},
                                                           {"2817", "10418"},
                                                           {"2817", "10454"},
                                                           {"2817", "10448"},
                                                           {"2817", "10444"},
                                                           {"2817", "10435"},
                                                           {"2817", "10432"},
                                                           {"2817", "11129"},
                                                           {"2817", "10421"},
                                                           {"2817", "10408"},
                                                           {"2817", "10409"},
                                                           {"2819", "11129"},
                                                           {"2819", "10448"},
                                                           {"2819", "10429"},
                                                           {"2819", "9101" },
                                                           {"2823", "10469"},
                                                           {"2823", "10479"},
                                                           {"2824", "10512"},
                                                           {"2824", "8531"},
                                                           {"2824", "10541"},
                                                           {"2824", "10524"},
                                                           {"2824", "10504"}};
    std::vector<std::set<int>> asserter ={
            {4},
            {34,44},
            {4,5,8,10,13,14,17,19,21,45},
            {2,11,19,45},
            {23,25},
            {0,34,36,38,44}
    };
    pop.processStudentsExams(studentsExams);
    ASSERT_EQ(examData->enrollment,asserter);
}

TEST_F(ExamDataPopulatorTest, CollisionLimit){
    ASSERT_EQ(examData->examsCollisions,examData->examsCollisionsLimit14);
}
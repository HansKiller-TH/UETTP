//
// Created by hansk on 25.09.2024.
//

#include "gtest/gtest.h"
#include "data_utils/ExamTTSolution.h"
#include "data_utils/ExamTTData.h"
#include "data_utils/ExamTTDataTMP.h"

TEST(ExamTTSolutionTest, copyTest){
    ExamTTDataTMP tmp;
    tmp.examID = {1,2,3,4};
    ExamTTData examTTData(tmp);
    ExamTTSolution solution(std::make_shared<ExamTTData>(examTTData));
    solution.examPeriod = {1,2,3};
    auto copy = solution;
    EXPECT_EQ(copy, solution);
    EXPECT_EQ(copy.examData,solution.examData);
    copy.examPeriod = {1,2,3,4,5};
    EXPECT_NE(copy,solution);
}

TEST(ExamTTSolutionTest, deepCopyTest){
    ExamTTDataTMP tmp;
    tmp.examID = {1,2,3,4};
    ExamTTData examTTData(tmp);
    ExamTTSolution solution(std::make_shared<ExamTTData>(examTTData));
    solution.examPeriod = {1,2,3};
    auto copy = solution.deepCopy();
    EXPECT_EQ(copy, solution);
    EXPECT_NE(copy.examData,solution.examData);
    EXPECT_EQ(*copy.examData,*solution.examData);
    EXPECT_EQ(copy.examData->examID,solution.examData->examID);
    copy.examPeriod = {1,2,3,4,5};
    EXPECT_NE(copy,solution);
}

TEST(ExamTTSolutionTest, deepCopyMakeSharedTest){
    ExamTTDataTMP tmp;
    tmp.examID = {1,2,3,4};
    ExamTTData examTTData(tmp);
    ExamTTSolution solution(std::make_shared<ExamTTData>(examTTData));
    solution.examPeriod = {1,2,3};
    solution.examDegreeOfSaturation = {0, 4, 2};
    std::cout << "obj: " << solution.examDegreeOfSaturation.size() << std::endl;
    auto sol_ptr = std::make_shared<ExamTTSolution>(solution);
    ASSERT_EQ(*sol_ptr, solution);
    std::cout << "before: " << sol_ptr->examDegreeOfSaturation.size() << std::endl;
    ASSERT_NE(sol_ptr->examDegreeOfSaturation.size(),0);
    auto copy_ptr = std::make_shared<ExamTTSolution>(*sol_ptr);
    std::cout << "after: " << copy_ptr->examDegreeOfSaturation.size() << std::endl;
    ASSERT_NE(sol_ptr, copy_ptr);
    ASSERT_EQ(*sol_ptr, *copy_ptr);
    ASSERT_EQ(sol_ptr->examDegreeOfSaturation, copy_ptr->examDegreeOfSaturation);

}

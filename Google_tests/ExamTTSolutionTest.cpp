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

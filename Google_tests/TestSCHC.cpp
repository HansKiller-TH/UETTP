//
// Created by hansk on 27.09.2024.
//
#include "gtest/gtest.h"
#include "data_utils/ExamTTData.h"
#include "data_utils/ExamTTFileManager.h"
#include "Algorithm/InitialSolution.h"
#include "Algorithm/SCHC.h"

TEST(SCHCTest, CtorTest){
    auto examDataPtr = ExamTTFileManager::importExamTTData("./Test-23WS.accdb");
    InitialSolution initialSolution(examDataPtr);
    initialSolution.random = true;
    auto solution = initialSolution.build();
    ASSERT_NE(solution->examPeriod.size(), 0);
    std::cout << "size before: " << solution->examDegreeOfSaturation.size() << std::endl;
    ASSERT_NE(solution->examDegreeOfSaturation.size(),0);
    SCHC algo(solution);
    std::cout << "size after: " << algo.currentSolution_->examDegreeOfSaturation.size() << std::endl;
    //SCHC algo(std::make_shared<ExamTTSolution>(solution->deepCopy()));
    //auto algo = std::make_shared<SCHC>(std::make_shared<ExamTTSolution>(solution->deepCopy()));
    algo.stopTime = 0.0;
    algo.random = true;
    algo.fullCollisionCost = true;
    algo.schc_all = true;
    algo.schc_acp = false;
    algo.schc_imp = false;
    algo.counterLimit = 500;
    ASSERT_EQ(*(algo.currentSolution_), *solution);
    ASSERT_EQ(algo.currentSolution_->examPeriod.size(), solution->examPeriod.size());
    ASSERT_NE(algo.currentSolution_->examDegreeOfSaturation.size(),0);
    ASSERT_EQ(algo.currentSolution_->examDegreeOfSaturation, solution->examDegreeOfSaturation);
}
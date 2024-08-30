//
// Created by hansk on 11.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCREENOUTPUT_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCREENOUTPUT_H

#include <iostream>
#include <vector>
#include <string>
#include "data_utils/ExamTTData.h"
#include "data_utils/ExamTTSolution.h"
#include "data_utils/RoomType.h"

class screenOutput {
public:
    static void setOut(const std::set<int> &s, std::string name);
    static void vectorOut(std::vector<int> vec, std::string name);
    static void vectorOut(std::vector<std::string> vec, std::string name);
    static void vectorOut(std::vector<std::vector<int>> vec, std::string name);
    static void solutionOut(ExamTTSolution &data);
    static void solutionDifference(const ExamTTSolution &data1, const ExamTTSolution &data2);
    static void printExamData(ExamTTSolution &data);

    static void vectorOut(const std::vector<std::set<int>> &vec, const std::string& name);

    static void vectorOut(const std::vector<RoomType> &vec, const std::string &name);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCREENOUTPUT_H

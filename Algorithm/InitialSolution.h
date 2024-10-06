//
// Created by hansk on 29.04.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_INITIALSOLUTION_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_INITIALSOLUTION_H

#include <utility>
#include <vector>
#include <iostream>
#include <memory>
#include <set>
#include "data_utils/ExamTTData.h"
#include "output_utils/screenOutput.h"
#include "ExamTTSolutionManipulator.h"

/**@brief Constructive heuristics to create an Initial Solution */
class InitialSolution {
public:
    explicit InitialSolution(const std::shared_ptr<ExamTTData> &examTTData) {
        manipulator = std::make_shared<ExamTTSolutionManipulator>(std::make_shared<ExamTTSolution>(examTTData));
    }

    /**@brief schedules exams in a certain order */
    std::shared_ptr<ExamTTSolution> build() {
        if (manipulator == nullptr)
            throw std::invalid_argument("ERROR: Pointer to ExamTTSolutionManipulator object is null");
        std::cout << "Building Initial Solution..." << std::endl;
        unscheduledExams = manipulator->getAllExams();
        while (!unscheduledExams.empty()) {
            if (!scheduleExam(getNextExam()))
                return {};
        }
        return manipulator->getSolution();
    }

    bool random = true;
private:
    bool largeExams = true;
    std::set<int> unscheduledExams;
    std::shared_ptr<ExamTTSolutionManipulator> manipulator;

    /** Exams are assigned to a period with sufficient rooms that satisfies the hard constraints */
    bool scheduleExam(const std::set<int> &exams);

    /**@brief picks the next exam according to its strategy. In this case DSatur */
    std::set<int> getNextExam();

};

#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_INITIALSOLUTION_H

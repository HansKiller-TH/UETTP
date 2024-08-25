//
// Created by hansk on 18.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONACCDBEXPORTER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONACCDBEXPORTER_H

#include <memory>
#include "ExamTTData.h"

class ExamTTSolutionAccdbExporter {
public:
    static void exportExamTTSolution(const std::shared_ptr<ExamTTData>& examData);
    static void exportExamTTSolution(const std::shared_ptr<ExamTTData>& examData, std::string &filePath);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONACCDBEXPORTER_H

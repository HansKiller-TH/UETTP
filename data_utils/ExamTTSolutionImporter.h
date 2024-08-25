//
// Created by hansk on 18.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONIMPORTER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONIMPORTER_H


#include <memory>
#include "ExamTTData.h"

class ExamTTSolutionImporter {
public:
    static std::shared_ptr<ExamTTData> import(std::shared_ptr<ExamTTData> examData, std::string &filename);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONIMPORTER_H

//
// Created by hansk on 10.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ILOGGER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ILOGGER_H
#include <memory>
#include "data_utils/ExamTTData.h"
#include "data_utils/ExamTTSolution.h"

class ILogger {
public:
    virtual void add(std::shared_ptr<ExamTTSolution> solution) = 0;
    virtual void setFileName(const std::string &configFilename) = 0;
    virtual void writeToFile() = 0;
};
#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ILOGGER_H

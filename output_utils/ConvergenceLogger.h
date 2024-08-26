//
// Created by hansk on 12.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_CONVERGENCELOGGER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_CONVERGENCELOGGER_H


#include "ILogger.h"

class ConvergenceLogger : ILogger {
public:
    void add(std::shared_ptr<ExamTTSolution> solution) override;
    std::vector<std::vector<std::pair<int, double>>> getData();

    void writeToFile() override;

    void setFileName(const std::string& configFilename) override;

    void reset();
private:
    std::vector<std::vector<std::pair<int, double>>> convergenceData;
    std::string filename;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_CONVERGENCELOGGER_H

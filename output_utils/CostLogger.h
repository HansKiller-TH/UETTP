//
// Created by hansk on 01.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_COSTLOGGER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_COSTLOGGER_H


#include <string>
#include <vector>
#include <memory>
#include "data_utils/ExamTTData.h"
#include "ILogger.h"

class CostLogger : ILogger {
public:
    void add(std::shared_ptr<ExamTTSolution> solution) override;
    std::vector<std::vector<int>> getData();

    void writeToFile() override;

    void setFileName(const std::string& configFilename) override;

    void reset();
private:
    std::vector<std::vector<int>> costData;
    std::vector<double> timeData;
    std::string filename;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_COSTLOGGER_H

//
// Created by hansk on 10.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATALOGGER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATALOGGER_H


#include <memory>
#include "data_utils/ExamTTData.h"
#include "ILogger.h"
#include "data_utils/ExamTTSolution.h"

class ExamDataLogger{
public:
    int runs = 0;
    void add(const std::shared_ptr<ExamTTSolution>& solution);
    void setFileName(const std::string &configFilename);
    void writeToFileBest();
    void writeToFileTimesCosts();

    struct CompareExamDataCost {
        bool operator()(const std::shared_ptr<ExamTTSolution>& e1, const std::shared_ptr<ExamTTSolution>& e2) const {
            return e1->cost < e2->cost;
        }
    };
    std::vector<std::shared_ptr<ExamTTSolution>> getData();
    void reset();
private:
    std::vector<std::shared_ptr<ExamTTSolution>> solutions;
    std::string filename;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATALOGGER_H

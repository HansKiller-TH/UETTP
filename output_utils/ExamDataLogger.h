//
// Created by hansk on 10.07.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATALOGGER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATALOGGER_H


#include <memory>
#include "data_utils/ExamTTData.h"
#include "ILogger.h"

class ExamDataLogger{
public:
    int runs = 0;
    void add(std::shared_ptr<ExamTTData> examData);
    void setFileName(const std::string &configFilename);
    void writeToFileBest();
    void writeToFileTimesCosts();

    struct CompareExamDataCost {
        bool operator()(const std::shared_ptr<ExamTTData>& e1, const std::shared_ptr<ExamTTData>& e2) const {
            return e1->cost < e2->cost;
        }
    };
    std::multiset<std::shared_ptr<ExamTTData>, CompareExamDataCost> getData();
    void reset();
private:
    std::multiset<std::shared_ptr<ExamTTData>,CompareExamDataCost> solutions;
    std::string filename;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATALOGGER_H

//
// Created by hansk on 06.06.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTFILEMANAGER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTFILEMANAGER_H


#include <memory>
#include "ExamTTData.h"
#include "ExamTTSolution.h"

class ExamTTFileManager {
public:
    static std::shared_ptr<ExamTTData> importExamTTData(const std::string &filepath);
    static std::shared_ptr<ExamTTData> importExamTTSolution(std::shared_ptr<ExamTTData> examData, std::string &filename);
    static void exportExamTTSolution(const std::shared_ptr<ExamTTSolution> &examTTSolution);
    static void exportExamTTSolution(const std::shared_ptr<ExamTTSolution> &examTTSolution, std::string &filePath);

};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTFILEMANAGER_H

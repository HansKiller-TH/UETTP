//
// Created by hansk on 06.06.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATAIMPORTER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATAIMPORTER_H


#include <memory>
#include "ExamTTData.h"

class ExamTTDataImporter {
public:
    static std::shared_ptr<ExamTTData> import(const std::string &filepath);

};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTDATAIMPORTER_H

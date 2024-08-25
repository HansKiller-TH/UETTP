//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ABSTRACTEXAMDATAFILEPARSER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ABSTRACTEXAMDATAFILEPARSER_H

#include <string>
#include <memory>
#include "IExamDataParser.h"
#include "ExamTTData.h"

class AbstractExamDataFileParser {
protected:
    const std::string FILE_PATH;
    explicit AbstractExamDataFileParser(std::string filePath) : FILE_PATH(std::move(filePath)){}
public:
    ~AbstractExamDataFileParser() = default;

};
#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ABSTRACTEXAMDATAFILEPARSER_H

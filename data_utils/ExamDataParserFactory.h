//
// Created by hansk on 05.06.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAPARSERFACTORY_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAPARSERFACTORY_H


#include "IExamDataParser.h"
#include "ExamDataXmlParser.h"

class ExamDataParserFactory {
public:
    ExamDataParserFactory() = default;

    static std::unique_ptr<IExamDataParser> createParser(const std::string &filepath);

private:

    static std::string getExtensionLowerWithoutDot(const std::string &filepath);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAPARSERFACTORY_H

//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_IEXAMDATAPARSER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_IEXAMDATAPARSER_H

#include "ExamTTData.h"
#include "ExamTTDataBuilder.h"

class IExamDataParser {
public:
    virtual std::vector<std::vector<std::string>> parsePeriods() = 0;

    virtual std::vector<std::vector<std::string>> parseRooms() = 0;

    virtual std::vector<std::vector<std::string>> parseRoomsValidPeriods() = 0;

    virtual std::vector<std::vector<std::string>> parseExams() = 0;

    virtual std::vector<std::vector<std::string>> parseExamsValidPeriods() = 0;

    virtual std::vector<std::vector<std::string>> parseExamsValidRooms() = 0;

    virtual std::vector<std::vector<std::string>> parseStudentsExams() = 0;

    virtual std::vector<std::vector<std::string>> parseSamePeriod() = 0;

    virtual std::vector<std::vector<std::string>> parseSolutionExamPeriod() = 0;

    virtual std::vector<std::vector<std::string>> parseSolutionExamRooms() = 0;

    virtual ~IExamDataParser() = default;
};

#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_IEXAMDATAPARSER_H

//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATATXTPARSER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATATXTPARSER_H


#include "AbstractExamDataFileParser.h"
#include "ExamTTData.h"

class ExamDataTxtParser : public IExamDataParser {
public:
    explicit ExamDataTxtParser(const std::string &filePath){}

    std::vector<std::vector<std::string>> parsePeriods() override;

    std::vector<std::vector<std::string>> parseRooms() override;

    std::vector<std::vector<std::string>> parseRoomsValidPeriods() override;

    std::vector<std::vector<std::string>> parseExams() override;

    std::vector<std::vector<std::string>> parseExamsValidPeriods() override;

    std::vector<std::vector<std::string>> parseExamsValidRooms() override;

    std::vector<std::vector<std::string>> parseStudentsExams() override;

    std::vector<std::vector<std::string>> parseSamePeriod() override;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATATXTPARSER_H

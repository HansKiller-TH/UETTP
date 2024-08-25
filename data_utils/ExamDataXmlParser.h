//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAXMLPARSER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAXMLPARSER_H


#include <string>
#include <memory>
#include <stdexcept>
#include "ExamTTData.h"
#include "IExamDataParser.h"
#include "file_utils/tinyxml2/tinyxml2.h"

class ExamDataXmlParser : public IExamDataParser {
public:
    explicit ExamDataXmlParser(const std::string &filepath){
        tinyxml2::XMLError eResult = doc.LoadFile(filepath.c_str());
        if (eResult != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("Error loading XML file!");
        root = doc.FirstChildElement("examtt");
        if (root == nullptr)
            throw std::runtime_error("XML root empty or not examtt");
    }

    std::vector<std::vector<std::string>> parsePeriods() override;

    std::vector<std::vector<std::string>> parseRooms() override;

    std::vector<std::vector<std::string>> parseRoomsValidPeriods() override;

    std::vector<std::vector<std::string>> parseExams() override;

    std::vector<std::vector<std::string>> parseExamsValidPeriods() override;

    std::vector<std::vector<std::string>> parseExamsValidRooms() override;

    std::vector<std::vector<std::string>> parseStudentsExams() override;

    std::vector<std::vector<std::string>> parseSamePeriod() override;

    std::vector<std::vector<std::string>> parseSolutionExamPeriod() override;

    std::vector<std::vector<std::string>> parseSolutionExamRooms() override;

private:
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root;
    static std::string toString(const char *constChar);

    static std::vector<std::vector<std::string>>
    getChildrensFirstAttribute(tinyxml2::XMLElement *start, const char *parentsName, const char *childsName);

    static std::vector<std::vector<std::string>>
    getSiblingsAllAttributes(tinyxml2::XMLElement *start, const char *siblingsName);
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAXMLPARSER_H

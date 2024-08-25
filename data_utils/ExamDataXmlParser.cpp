//
// Created by hansk on 09.05.2024.
//

#include <stdexcept>
#include "ExamDataXmlParser.h"

//TODO: get element attributes by name to ensure order
std::vector<std::vector<std::string>> ExamDataXmlParser::parsePeriods() {
    auto *start = root->FirstChildElement("periods")->FirstChildElement("period");
    return getSiblingsAllAttributes(start, "period");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseRooms() {
    auto start = root->FirstChildElement("rooms")->FirstChildElement("room");
    return getSiblingsAllAttributes(start, "room");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseRoomsValidPeriods() {
    auto start = root->FirstChildElement("rooms")->FirstChildElement("room");
    return getChildrensFirstAttribute(start,"room","period");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseExams() {
    auto start = root->FirstChildElement("exams")->FirstChildElement("exam");
    return getSiblingsAllAttributes(start, "exam");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseExamsValidPeriods() {
    auto start = root->FirstChildElement("exams")->FirstChildElement("exam");
    return getChildrensFirstAttribute(start, "exam", "period");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseExamsValidRooms() {
    auto start = root->FirstChildElement("exams")->FirstChildElement("exam");
    return getChildrensFirstAttribute(start, "exam", "room");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseStudentsExams() {
    auto start = root->FirstChildElement("students")->FirstChildElement("student");
    return getChildrensFirstAttribute(start, "student", "exam");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseSamePeriod() {
    auto start = root->FirstChildElement("constraints")->FirstChildElement("same-period");
    return getChildrensFirstAttribute(start, "same-period", "exam");
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseSolutionExamPeriod() {
    return std::vector<std::vector<std::string>>();
}

std::vector<std::vector<std::string>> ExamDataXmlParser::parseSolutionExamRooms() {
    return std::vector<std::vector<std::string>>();
}

std::vector<std::vector<std::string>>
ExamDataXmlParser::getSiblingsAllAttributes(tinyxml2::XMLElement *start, const char *siblingsName) {
    std::vector<std::vector<std::string>> elements;
    for (auto element = start; element != nullptr; element = element->NextSiblingElement(siblingsName)) {
        elements.emplace_back();
        for (auto attr = element->FirstAttribute(); attr != nullptr; attr = attr->Next())
            elements.back().emplace_back(toString(attr->Value()));
    }
    return elements;
}

std::vector<std::vector<std::string>>
ExamDataXmlParser::getChildrensFirstAttribute(tinyxml2::XMLElement *start, const char *parentsName, const char *childsName) {
    std::vector<std::vector<std::string>> elements;
    for (auto parent = start; parent != nullptr; parent = parent->NextSiblingElement(parentsName)) {
        start = parent->FirstChildElement(childsName);
        for (auto child = start; child != nullptr; child = child->NextSiblingElement(childsName)) {
            elements.emplace_back();
            elements.back().emplace_back(toString(parent->FirstAttribute()->Value()));
            elements.back().emplace_back(toString(child->FirstAttribute()->Value()));
        }
    }
    return elements;
}

std::string ExamDataXmlParser::toString(const char *constChar) {
    if (constChar == nullptr)
        return "-1";
    return {constChar};
}



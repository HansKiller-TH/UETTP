//
// Created by hansk on 06.06.2024.
//

#include <iostream>
#include <filesystem>
#include "ExamTTDataImporter.h"
#include "ExamDataPopulator.h"
#include "ExamDataParserFactory.h"

std::shared_ptr<ExamTTData> ExamTTDataImporter::import(const std::string &filepath) {
    std::cout << "Importing..." << std::endl;
    //std::cout << "Creating ExamTTData..." << std::endl;
    auto examData = std::make_shared<ExamTTData>();
    ExamDataPopulator examDataPopulator(examData);
    auto parser = ExamDataParserFactory::createParser(filepath);
    // methods that handle a combination of Data depend on the methods that process those Data being called beforehand
    examDataPopulator.processPeriods(parser->parsePeriods());
    examDataPopulator.processRooms(parser->parseRooms());
    // depends on parsePeriods and parseRooms be called beforehand
    examDataPopulator.processRoomsValidPeriods(parser->parseRoomsValidPeriods());
    examDataPopulator.processExams(parser->parseExams());
    // depends on parsePeriods and parseExams be called beforehand
    examDataPopulator.processExamValidPeriods(parser->parseExamsValidPeriods());
    // depends on parseExams and parseRooms be called beforehand
    examDataPopulator.processExamsValidRooms(parser->parseExamsValidRooms());
    // depends on parseExams be called beforehand
    examDataPopulator.processStudentsExams(parser->parseStudentsExams());
    // depends on parseExams be called beforehand
    examDataPopulator.processSamePeriods(parser->parseSamePeriod());
    // depends on parseExams be called beforehand
    examDataPopulator.createCollisionsFromEnrollment();
    examDataPopulator.createCollisionMatrixLimitEnrolment();
    // depends on parsePeriods and parseExams be called beforehand
    examDataPopulator.setUpSolutionDependentData();
    examData->setMeta(filepath);
    return std::move(examData);
}

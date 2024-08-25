//
// Created by hansk on 06.06.2024.
//

#include <iostream>
#include <filesystem>
#include "ExamTTFileManager.h"
#include "ExamDataPopulator.h"
#include "ExamDataParserFactory.h"
#include "Algorithm/ExamDataManipulator.h"
#include "vectorUtils/VectorUtils.h"
#include "file_utils/AccdbFileHandler.h"

std::shared_ptr<ExamTTData> ExamTTFileManager::importExamTTData(const std::string &filepath) {
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
    // examDataPopulator.setUpSolutionDependentData();
    examData->setMeta(filepath);
    return std::move(examData);
}

std::shared_ptr<ExamTTData>
ExamTTFileManager::importExamTTSolution(std::shared_ptr<ExamTTData> examData, std::string &filename) {
    ExamDataManipulator manipulator(examData);
    auto parser = ExamDataParserFactory::createParser(filename);
    for (const auto &row: parser->parseSolutionExamPeriod()) {
        int examIndex = VectorUtils::indexForValue(examData->examID, row.at(0));
        if (examIndex == -1)
            continue;
        int periodIndex = VectorUtils::indexForValue(examData->periodID, row.at(1));
        if (periodIndex == -1)
            continue;
        manipulator.moveExamToPeriod(examIndex, periodIndex);
    }
    for (const auto &row: parser->parseSolutionExamRooms()) {
        int examIndex = VectorUtils::indexForValue(examData->examID, row.at(0));
        if (examIndex == -1)
            continue;
        int roomIndex = VectorUtils::indexForValue(examData->roomID, row.at(1));
        if (roomIndex == -1)
            continue;

        int periodIndex = examData->examPeriod.at(examIndex);

        if (examData->periodRoomsAvailability.at(periodIndex).at(roomIndex) == -1) {
            std::cout << "period: " << std::to_string(examData->periodID.at(periodIndex)) << ", exam: "
                      << std::to_string(examData->examID.at(examIndex)) << ", room: "
                      << std::to_string(examData->roomID.at(roomIndex)) << std::endl;
            throw std::runtime_error("tried assigning an invalid room");
        }
        // Practical Solution allows for very small exam sizes to be placed in the same room with another exam by the same lecturer
/*        if (examData->periodRoomsAvailability.at(periodIndex).at(roomIndex) == 0) {
            std::cout << "period: " << std::to_string(examData->periodID.at(periodIndex)) << ", exam: "
                      << std::to_string(examData->examID.at(examIndex)) << ", room: "
                      << std::to_string(examData->roomID.at(roomIndex)) << std::endl;
            throw std::runtime_error("tried assigning an unavailable room");
        }*/
        if (examData->roomType.at(roomIndex) != ExamTTData::RoomType::Online)
            examData->periodRoomsAvailability.at(periodIndex).at(roomIndex) = 0;
        examData->examRooms.at(examIndex).insert(roomIndex);
    }
    return examData;
}

void ExamTTFileManager::exportExamTTSolution(const std::shared_ptr<ExamTTSolution> &examTTSolution) {
    exportExamTTSolution(examTTSolution, examTTSolution->examData->filePath);
}

void ExamTTFileManager::exportExamTTSolution(const std::shared_ptr<ExamTTSolution> &examTTSolution, std::string &filePath) {
    AccdbFileHandler fh(filePath);
    // fh.clearTable("Pruefungseinheit");
    // fh.clearTable("Pruefungstermin");

    std::vector<std::vector<std::string>> termin;
    for (int i = 0; i < examTTSolution->examPeriod.size(); ++i) {
        std::vector<std::string> row;
        int periodIndex = examTTSolution->examPeriod.at(i);
        row.emplace_back(std::to_string(examTTSolution->examData->examID.at(i)));
        row.emplace_back(std::to_string(examTTSolution->examData->periodSlot.at(periodIndex)));
        row.emplace_back(std::to_string(examTTSolution->examData->periodDay.at(periodIndex)));
        row.emplace_back(std::to_string(examTTSolution->examData->periodID.at(periodIndex)));
        termin.emplace_back(row);
    }
    fh.writeMSAccessFile("Pruefungstermin", termin);

    std::vector<std::vector<std::string>> raum;
    int id = 1;
    for (int i = 0; i < examTTSolution->examPeriod.size(); ++i) {
        auto rooms = examTTSolution->examRooms.at(i);
        for (auto &roomIndex:rooms) {
            std::vector<std::string> row;
            // Pruefungseinheit id
            row.emplace_back(std::to_string(id++));
            // Raum_id
            row.emplace_back(std::to_string(examTTSolution->examData->roomID.at(roomIndex)));
            // Pruefungstermin_id (not an error here)
            row.emplace_back(std::to_string(examTTSolution->examData->examID.at(i)));
            // We don't have any Hinweise, so we write an empty string for that column.
            row.emplace_back("\'\'");
            raum.emplace_back(row);
        }
    }
    fh.writeMSAccessFile("Pruefungseinheit",raum);
}

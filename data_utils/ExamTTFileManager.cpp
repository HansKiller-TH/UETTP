//
// Created by hansk on 06.06.2024.
//

#include <iostream>
#include <filesystem>
#include "ExamTTFileManager.h"
#include "ExamTTDataBuilder.h"
#include "ExamDataParserFactory.h"
#include "Algorithm/ExamTTSolutionManipulator.h"
#include "vectorUtils/VectorUtils.h"
#include "file_utils/AccdbFileHandler.h"

std::shared_ptr<ExamTTData> ExamTTFileManager::importExamTTData(const std::string &filepath) {
    std::cout << "Importing..." << std::endl;
    auto parser = ExamDataParserFactory::createParser(filepath);
    ExamTTDataBuilder examTTDataBuilder;
    return examTTDataBuilder.processPeriods(parser->parsePeriods())
    .processRooms(parser->parseRooms())
    .processRoomsValidPeriods(parser->parseRoomsValidPeriods())
    .processExams(parser->parseExams())
    .processExamValidPeriods(parser->parseExamsValidPeriods())
    .processExamsValidRooms(parser->parseExamsValidRooms())
    .processStudentsExams(parser->parseStudentsExams())
    .processSamePeriods(parser->parseSamePeriod())
    .createCollisionsFromEnrollment()
    .build();
}

std::shared_ptr<ExamTTSolution>
ExamTTFileManager::importExamTTSolution(const std::shared_ptr<ExamTTData>& examData, std::string &filename) {
    auto solution = std::make_shared<ExamTTSolution>(examData);
    ExamTTSolutionManipulator manipulator(solution);
    auto parser = ExamDataParserFactory::createParser(filename);
    for (const auto &row: parser->parseSolutionExamPeriod()) {
        int examIndex = VectorUtils::indexForValue(solution->examData->examID, row.at(0));
        if (examIndex == -1)
            continue;
        int periodIndex = VectorUtils::indexForValue(solution->examData->periodID, row.at(1));
        if (periodIndex == -1)
            continue;
        manipulator.moveExamToPeriod(examIndex, periodIndex);
    }
    for (const auto &row: parser->parseSolutionExamRooms()) {
        int examIndex = VectorUtils::indexForValue(solution->examData->examID, row.at(0));
        if (examIndex == -1)
            continue;
        int roomIndex = VectorUtils::indexForValue(solution->examData->roomID, row.at(1));
        if (roomIndex == -1)
            continue;

        int periodIndex = solution->examPeriod.at(examIndex);

        if (solution->periodRoomsAvailability.at(periodIndex).at(roomIndex) == -1) {
            std::cout << "period: " << std::to_string(solution->examData->periodID.at(periodIndex)) << ", exam: "
                      << std::to_string(solution->examData->examID.at(examIndex)) << ", room: "
                      << std::to_string(solution->examData->roomID.at(roomIndex)) << std::endl;
            throw std::runtime_error("tried assigning an invalid room");
        }
        // Practical Solution allows for very small exam sizes to be placed in the same room with another exam by the same lecturer
/*        if (examDataTMP->periodRoomsAvailability.at(periodIndex).at(roomIndex) == 0) {
            std::cout << "period: " << std::to_string(examDataTMP->periodID.at(periodIndex)) << ", exam: "
                      << std::to_string(examDataTMP->examID.at(examIndex)) << ", room: "
                      << std::to_string(examDataTMP->roomID.at(roomIndex)) << std::endl;
            throw std::runtime_error("tried assigning an unavailable room");
        }*/
        if (solution->examData->roomType.at(roomIndex) != RoomType::Online)
            solution->periodRoomsAvailability.at(periodIndex).at(roomIndex) = 0;
        solution->examRooms.at(examIndex).insert(roomIndex);
    }
    return solution;
}

void ExamTTFileManager::exportExamTTSolution(const std::shared_ptr<ExamTTSolution> &examTTSolution) {
    exportExamTTSolution(examTTSolution, examTTSolution->examData->filePath);
}

void ExamTTFileManager::exportExamTTSolution(const std::shared_ptr<ExamTTSolution> &examTTSolution, const std::string &filePath) {
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

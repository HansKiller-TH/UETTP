//
// Created by hansk on 18.07.2024.
//

#include <iostream>
#include "ExamTTSolutionImporter.h"
#include "ExamDataParserFactory.h"
#include "Algorithm/ExamDataManipulator.h"
#include "vectorUtils/VectorUtils.h"

std::shared_ptr<ExamTTData>
ExamTTSolutionImporter::import(std::shared_ptr<ExamTTData> examData, std::string &filename) {
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
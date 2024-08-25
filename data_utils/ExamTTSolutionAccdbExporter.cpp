//
// Created by hansk on 18.07.2024.
//

#include "ExamTTSolutionAccdbExporter.h"
#include "file_utils/AccdbFileHandler.h"

void ExamTTSolutionAccdbExporter::exportExamTTSolution(const std::shared_ptr<ExamTTData> &examData) {
    exportExamTTSolution(examData, examData->filePath);
}

void ExamTTSolutionAccdbExporter::exportExamTTSolution(const std::shared_ptr <ExamTTData>& examData, std::string &filePath) {
    AccdbFileHandler fh(filePath);
    // fh.clearTable("Pruefungseinheit");
    // fh.clearTable("Pruefungstermin");

    std::vector<std::vector<std::string>> termin;
    for (int i = 0; i < examData->examPeriod.size(); ++i) {
        std::vector<std::string> row;
        int periodIndex = examData->examPeriod.at(i);
        row.emplace_back(std::to_string(examData->examID.at(i)));
        row.emplace_back(std::to_string(examData->periodSlot.at(periodIndex)));
        row.emplace_back(std::to_string(examData->periodDay.at(periodIndex)));
        row.emplace_back(std::to_string(examData->periodID.at(periodIndex)));
        termin.emplace_back(row);
    }
    fh.writeMSAccessFile("Pruefungstermin", termin);

    std::vector<std::vector<std::string>> raum;
    int id = 1;
    for (int i = 0; i < examData->examPeriod.size(); ++i) {
        auto rooms = examData->examRooms.at(i);
        for (auto &roomIndex:rooms) {
            std::vector<std::string> row;
            // Pruefungseinheit id
            row.emplace_back(std::to_string(id++));
            // Raum_id
            row.emplace_back(std::to_string(examData->roomID.at(roomIndex)));
            // Pruefungstermin_id (not an error here)
            row.emplace_back(std::to_string(examData->examID.at(i)));
            // We don't have any Hinweise, so we write an empty string for that column.
            row.emplace_back("\'\'");
            raum.emplace_back(row);
        }
    }
    fh.writeMSAccessFile("Pruefungseinheit",raum);
}

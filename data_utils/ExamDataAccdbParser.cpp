//
// Created by hansk on 09.05.2024.
//

#include "ExamDataAccdbParser.h"

std::vector<std::vector<std::string>> ExamDataAccdbParser::parsePeriods() {
    auto selection = fh.readMSAccessFile("SELECT PruefungszeitID, PruefungstagID, ZeitfensterID, Woche, Datum FROM Pruefungszeiten ORDER BY PruefungszeitID");
    for (auto &row:selection)
        row.at(4) = row.at(4).substr(0,10);
    return selection;
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseRooms() {
    return fh.readMSAccessFile("SELECT RaumID, Bezeichnung, PGroesse, Typ FROM Raeume ORDER BY RaumID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseRoomsValidPeriods() {
    return fh.readMSAccessFile("SELECT RaumID, PruefungszeitID FROM Raumverfuegbarkeit ORDER BY RaumID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseExams() {
    return fh.readMSAccessFile("SELECT PruefungID, BezeichnungKurz, Anmeldungen FROM Pruefungen ORDER BY PruefungID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseExamsValidPeriods() {
    return fh.readMSAccessFile("SELECT PruefungID, PruefungszeitID FROM PruefungZulaessigeZeiten ORDER BY PruefungID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseExamsValidRooms() {
    return fh.readMSAccessFile("SELECT PruefungID, RaumID FROM PruefungZulaessigeRaeume ORDER BY PruefungID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseStudentsExams() {
    return fh.readMSAccessFile("SELECT StudentID, PruefungID FROM Pruefungsanmeldung ORDER BY StudentID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseSamePeriod() {
    return fh.readMSAccessFile("SELECT FachID, PruefungID FROM Gleichzeitig ORDER BY FachID");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseSolutionExamPeriod() {
    return fh.readMSAccessFile("SELECT id, PruefungszeitID FROM Pruefungstermin ORDER BY id");
}

std::vector<std::vector<std::string>> ExamDataAccdbParser::parseSolutionExamRooms() {
    // Column name Pruefungstermin_id is correct here and actually contains the exam ID
    return fh.readMSAccessFile("SELECT Pruefungstermin_id, Raum_id FROM Pruefungseinheit ORDER BY Pruefungstermin_id");
}

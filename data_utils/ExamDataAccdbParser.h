//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAACCDBPARSER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAACCDBPARSER_H


//#include <memory>
#include "IExamDataParser.h"
#include "file_utils/AccdbFileHandler.h"

/**
 * @class ExamDataAccdbParser
 * @brief Implementation of the IExamDataParser interface for parsing exam data from an ACCDB file.
 *
 * This class provides methods to parse different types of exam data such as periods, rooms, exams,
 * students exams, same periods, solution_ exam periods, and solution_ exam rooms.
 *
 * @note The ExamDataAccdbParser class requires the AccdbFileHandler class to read the ACCDB file.
 */
class ExamDataAccdbParser : public IExamDataParser {
public:
    /**
     * @class ExamDataAccdbParser
     * @brief Implementation of the IExamDataParser interface for parsing exam data from an ACCDB file.
     *
     * This class provides methods to parse different types of exam data such as periods, rooms, exams,
     * students exams, same periods, solution_ exam periods, and solution_ exam rooms.
     *
     * @note The ExamDataAccdbParser class requires the AccdbFileHandler class to read the ACCDB file.
     */
    explicit ExamDataAccdbParser(const std::string &filepath) : fh(filepath){}

    /**
     * @brief Parses the periods data from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the periods data, including the period ID, day ID, time slot ID, week, and date.
     *
     * @return A vector of vector of strings containing the periods data.
     */
    std::vector<std::vector<std::string>> parsePeriods() override;

    /**
     * @brief Parses the rooms data from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the rooms data, including the room ID, its description, size, and type.
     *
     * @return A vector of vector of strings containing the rooms data.
     */
    std::vector<std::vector<std::string>> parseRooms() override;

    /**
     * @brief Parses the valid periods for each room from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the valid periods for each room, including the room ID and the corresponding exam period ID.
     *
     * @return A vector of vector of strings containing the room IDs and their corresponding valid exam period IDs.
     */
    std::vector<std::vector<std::string>> parseRoomsValidPeriods() override;

    /**
     * @brief Parses the exams data from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the exams data, including the exam ID, short description, and number of registrations.
     *
     * @return A vector of vector of strings containing the exams data.
     */
    std::vector<std::vector<std::string>> parseExams() override;

    /**
     * @brief Parses the valid periods for each exam from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the valid periods for each exam, including the exam ID and the corresponding exam period ID.
     *
     * @return A vector of vector of strings containing the exam IDs and their corresponding valid exam period IDs.
     */
    std::vector<std::vector<std::string>> parseExamsValidPeriods() override;

    /**
     * @brief Parses the valid rooms for each exam from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the valid rooms for each exam, including the exam ID and the corresponding room ID.
     *
     * @return A vector of vector of strings containing the exam IDs and their corresponding valid room IDs.
     */
    std::vector<std::vector<std::string>> parseExamsValidRooms() override;

    /**
     * @brief Parses the students exams data from an ACCDB file.
     *
     * This method is part of the ExamDataAccdbParser class that implements the IExamDataParser interface.
     * It reads the ACCDB file and retrieves the students exams data, including the StudentID, and PruefungID (exam ID),
     * ordered by StudentID.
     *
     * @return A vector of vector of strings containing the students exams data.
     */
    std::vector<std::vector<std::string>> parseStudentsExams() override;

    /**
     * @class ExamDataAccdbParser
     * @brief Implementation of the IExamDataParser interface for parsing exam data from an ACCDB file.
     *
     * This class provides methods to parse different types of exam data such as periods, rooms, exams,
     * students exams, same periods, solution_ exam periods, and solution_ exam rooms.
     *
     * @note The ExamDataAccdbParser class requires the AccdbFileHandler class to read the ACCDB file.
     */
    std::vector<std::vector<std::string>> parseSamePeriod() override;

    std::vector<std::vector<std::string>> parseSolutionExamPeriod() override;

    std::vector<std::vector<std::string>> parseSolutionExamRooms() override;

private:
    AccdbFileHandler fh;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMDATAACCDBPARSER_H

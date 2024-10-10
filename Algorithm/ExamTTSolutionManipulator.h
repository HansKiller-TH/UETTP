//
// Created by hansk on 21.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONMANIPULATOR_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONMANIPULATOR_H

#include <set>
#include <random>
#include <memory>
#include "data_utils/ExamTTData.h"
#include "data_utils/ExamTTSolution.h"
#include "PeriodChange.h"

/**
 * @class ExamTTSolutionManipulator
 * @brief Safely manipulates or creates a solution_ from an ExamTTData object.
 */
class ExamTTSolutionManipulator {
public:
    ExamTTSolutionManipulator():gen(std::random_device{}()) {
        std::random_device rd;
        gen.seed(rd());
    }

    explicit ExamTTSolutionManipulator(
            std::shared_ptr<ExamTTSolution> examTTSolution) : solution_(std::move(examTTSolution)), gen(std::random_device{}()) {
        std::random_device rd;
        gen.seed(rd());
    }

    /**
     * @brief Set the Exam Data object
     *
     * This function sets the examData_ member variable of the ExamTTSolutionManipulator class.
     *
     * @param solution A shared pointer to an ExamTTData object
     */
    void setSolution(const std::shared_ptr<ExamTTSolution>& solution);

    /**
     * @fn std::shared_ptr<ExamTTData> ExamTTSolutionManipulator::getSolution()
     * @brief Retrieves the ExamTTData object.
     *
     * This function returns a shared pointer to the ExamTTData object stored in the ExamTTSolutionManipulator class.
     *
     * @return A shared pointer to the ExamTTData object.
     */
    [[nodiscard]] std::shared_ptr<ExamTTSolution> getSolution();

    /**
        * @brief Get a random exam index.
        *
        * This function returns a random exam index from the available exams in the `ExamTTData` object.
        *
        * @return An integer representing the index of the random exam.
        */
    int getRandomExam();

    /**
     * @brief Get a randomly selected exam and the exams that have to be in the same period with it.
     *
     * This function returns a set of exams, including a randomly selected exam and the exams that have to be in the same period with it,
     * from the available exams in the ExamTTData object.
     *
     * @return A set of integers representing the indexes of the randomly selected exam and the exams that have to be in the same period with it.
     */
    std::set<int> getRandomExamWithConnected();

    /**
     * @brief Get a randomly selected exam, the exams that have to be in the same period with it and their period.
     *
     * This function returns a pair of a set of exams, including a randomly selected exam and the exams that have to be in the same period with it, and their period.
     *
     * @return A pair consisting of a set of integers representing the indexes of the randomly selected exam and the exams that have to be in the same period with it, and an integer representing their period.
     */
    std::pair<std::set<int>,int> getRandomExamWithConnectedWithPeriod();

    /**
     * @brief Remove the exam with the largest degree of saturation from the set of unscheduled exams.
     *
     * This function removes the exam with the largest degree of saturation from the set of unscheduled exams.
     * If equal the exam with the larger degree.
     *
     * @param unscheduledExams A reference to a set of integers representing the unscheduled exams.
     * @return A set of integers representing the exams that have been removed from the unscheduled exams set.
     */
    std::set<int> removeExamWithLargestDegreeOfSaturation(std::set<int> &unscheduledExams);

    std::set<int> removeExamDSatur(std::set<int> &unscheduledExams);

    /**
     * @brief Remove the exam with the largest number of enrolled students and exams that have to be in the same period with it from the set of unscheduled exams.
     *
     * This function removes the exam with the largest number of enrolled students from the set of unscheduled exams.
     * If there are multiple largest exams, it removes the one with the largest index.
     *
     * @param unscheduledExams A reference to a set of integers representing the unscheduled exams.
     * @return A set of integers representing the exams that have been removed from the unscheduled exams set.
     */
    std::set<int> removeLargestExam(std::set<int> &unscheduledExams);

    /**
     * @brief Get a random period.
     *
     * This function returns a random period from the available periods in the ExamTTData object.
     *
     * @return An integer representing the index of the random period.
     */
    int getRandomPeriod();

    std::set<int> getRandomRoomsForExam(int exam, const std::vector<int> &roomsAvailability,
                                        int randomSampleSize = 0);

    /**
     * @fn void tryReassignRoomsToExamSamePeriod(const int &exam, std::set<int> &rooms)
     * @brief Reassigns rooms to an exam in the same period.
     *
     * This function reassigns the rooms of a specified exam to a new set of rooms in the same period. It updates the room availability information in the ExamTTData object.
     *
     * @param exam An integer representing the index of the exam to which the rooms will be reassigned.
     * @param rooms A reference to a set of integers representing the new set of rooms for the exam.
     */
    bool tryReassignRoomsToExamSamePeriod(int randomSampleSize, int exam);

    /**
     * @brief Attempt to switch the used rooms between two periods.
     *
     * This function attempts to switch the used rooms between two periods. It checks if it is possible to switch the rooms and if so, it performs the switch and updates the room availability information in the ExamTTData object.
     *
     * @param first An integer representing the index of the first period.
     * @param second An integer representing the index of the second period.
     * @return True if the switch was successful, false otherwise.
     */
    bool trySwitchUsedRooms(const PeriodChange &first, const PeriodChange &second);

    /**
     * Attempts to assign random rooms to each exam in a given period, ensuring no room conflicts.
     *
     * @param first The first period change, containing the period and exams to move in and out.
     * @param second Optional second period change, defaulting to a no-op if not provided.
     * @returns true if rooms are successfully assigned for all exams in both periods, false otherwise.
     *
     * @throws std::runtime_error if an invalid or unavailable room is attempted to be assigned.
     */
    bool
    tryAssignRandomRooms(int randomSampleSize, const PeriodChange &first,
                         const PeriodChange &second = PeriodChange());

    /**
     * @brief Moves an exam to a specified period.
     *
     * This function moves the given exam to the specified period in the exam data.
     * It updates the exam period, period exam collisions, and exam degree of saturation.
     * Moving an exam to period -1 will unschedule the exam.
     * If the old period is the same as the specified period, or if the exam is invalid (exam == -1),
     * no changes are made.
     *
     * @param exam   The exam to be moved.
     * @param period The target period to move the exam to.
     * @return       None.
     */
    void moveExamToPeriod(const int &exam, const int &period);

    /**
     * @brief Moves the given exams to the specified period.
     *
     * This function allows you to move a set of exams to a specific period.
     * The exams to be moved are specified using a set of integers, where each
     * integer represents the index of an exam. The target period is specified
     * using an integer value.
     *
     * If the given set of exams is empty, the function will return immediately
     * without performing any move.
     *
     * The function iterates through each exam in the set and calls the
     * "moveExamToPeriod" function to perform the actual move operation.
     *
     * @param exams A constant reference to a set of integers representing the exams to be moved.
     * @param period The target period to move the exams to.
     *
     * @return None.
     */
    void moveExamsToPeriod(const PeriodChange &first, const PeriodChange &second = PeriodChange());

    /**
     * @brief Performs the Kempe chain swap algorithm on two sets of exams.
     *
     * This function applies the Kempe chain swap algorithm to two sets of exams: examsFirst and examsSecond.
     * The algorithm aims to minimize the number of exams to be rescheduled.
     *
     * @param displacedFirst A reference to a set of exams that are displaced due to conflicts with examsFirst.
     * @param displacedSecond A reference to a set of exams that are displaced due to conflicts with examsSecond.
     * @param examsFirst A reference to the set of exams that remain in first period.
     * @param examsSecond A reference to the set of exams remain in second period.
     *
     * @return None.
     *
     * @note This function assumes that the set of exams in displacedFirst and examsFirst has been filled before calling this function.
     * @note This function assumes that the set of exams in displacedSecond and examsSecond has been filled before calling this function.
     * @note This function modifies the sets displacedFirst and displacedSecond by inserting new exams.
     * @note This function iteratively applies the Kempe chain swap algorithm until no more exams can be displaced.
     */
    void kempeChain(std::set<int> &displacedFirst, std::set<int> &displacedSecond,
                    std::set<int> &examsFirst, std::set<int> &examsSecond);

    /**
     * @brief Checks if any exams in the given set have a collision with the specified period.
     *
     * This function determines whether any of the exams in the specified set have a collision with the
     * given period. It returns true if there is at least one exam that collides with the period, and false
     * otherwise.
     *
     * @param exams A constant reference to a set of integers representing the exams.
     * @param relativePeriods The period to check for collisions.
     * @return true if there is at least one exam collision with the period, false otherwise.
     */
    bool hasAnyExamCollisionWithAnyPeriod(const PeriodChange &change, const std::set<int> &relativePeriods);

    /**
     * @brief Checks if any exam in the given set is invalid for the specified period.
     *
     * This function takes a set of exams and a period as parameters and checks if any of the exams in the set is invalid for the specified period.
     * An exam is considered invalid for a period if the corresponding value in the examPeriodsValidity data structure is 0.
     *
     * @param exams A constant reference to a set of integers representing the exams.
     * @param period An integer representing the period to check for validity.
     * @return A boolean value indicating whether any exam in the set is invalid for the specified period. Returns true if at least one exam is invalid, false otherwise.
     *
     * @note The function assumes that exams will be represented by unique integers.
     * @note The exams set and period value should be valid and within the range of the data structures used.
     */
    bool isAnyExamInvalidInPeriod(const std::set<int> &exams, const int &period);

    /**
     * @brief Get the exams in a given period.
     *
     * This function returns a set of exams that fall within the specified period.
     *
     * @param period The period for which to retrieve the exams.
     * @return A set of exams indexes that fall within the specified period.
     *
     * @note If the period is -1, an empty set is returned.
     *
     * @see ExamTTSolutionManipulator
     */
    [[nodiscard]] std::set<int> getExamsInPeriod(const int &period) const;

    /**
        * @brief Get the exams in a specific period, excluding specific exams.
        * @param without A set of exams to exclude from the result.
        * @return A set of exams in the specified period, excluding the provided exams.
        *
        * This function retrieves the exams that are scheduled in a specific period, while excluding
        * the provided set of exams from the result. The function takes a set of exams to be excluded as
        * input and returns a new set of exams without the excluded exams.
        */
    std::set<int> getExamsInPeriodWithout(const std::set<int> &without);

    /**
     * @brief Gets a set of all exams.
     *
     * This function returns a set containing the indexes of all exams.
     *
     * @return A set of integers representing the indexes of all exams.
     *
     * @note The order of the exams in the set may differ from the order of the exams in the underlying data structure.
     */
    [[nodiscard]] std::set<int> getAllExams() const;

    /**
     * @brief Get the valid exam periods for a set of exams.
     *
     * This function takes a set of exams and returns the valid periods for those exams.
     * The validity information for each exam is fetched from the examData_ member variable
     * and the valid periods are determined based on the validity values.
     *
     * @param exams The set of exams for which to get the valid periods.
     * @return A set of integers representing the valid periods for the given exams.
     *
     * @note The validity information for each exam is stored in the examData_->examPeriodsValidity map
     * where each exam is associated with a vector of validity values. A validity value of 1 indicates
     * that the corresponding period is valid for the exam.
     */
    std::set<int> getValidPeriodsForExams(const std::set<int> &exams);

    /**
     * @brief Calculates the previous period with the same day as the given period.
     *
     * This function checks if the given period is valid and if the day of the previous
     * period is the same as the day of the given period. If both conditions are met,
     * it returns the index of the previous period; otherwise, it returns -1.
     *
     * @param period The index of the period.
     * @return int The index of the previous period with the same day, or -1 if not found.
     */
    int getPreviousPeriodSameDay(const int &period);

    /**
     * @brief Returns the next period that occurs on the same day as the given period.
     *
     * This function takes an integer period as input and checks if the next period occurs on the same day as the given period.
     * If yes, it returns the index of the next period. If not, it returns -1.
     *
     * @param period An integer representing the index of the current period.
     * @return An integer representing the index of the next period on the same day, or -1 if no such period exists.
     */
    int getNextPeriodSameDay(const int &period);

private:
    std::shared_ptr<ExamTTSolution> solution_;
    std::mt19937 gen;

    /**
     * @brief Gets the available valid rooms for a given exam.
     *
     * This function retrieves the available valid rooms for a specified exam based on the provided rooms availability.
     * The availability of a room is determined by the value at the corresponding index in the `roomsAvailability` vector.
     * The function uses the `examData_->examRoomsValidity` member variable to obtain the validity of rooms for the specified exam.
     *
     * @param exam The index of the exam.
     * @param roomsAvailability The availability of rooms.
     * @return A set containing the available valid rooms indexes for the exam.
     */
    std::set<int> getAvailableValidRoomsForExam(const int &exam, const std::vector<int> &roomsAvailability);

    /**
     * @brief Extracts exams from the input set that have collisions with the inserts and the exams that have to be in the same period as the extracted exams.
     *
     * This function takes two sets of integers as input: inserts and exams. It extracts the exams from the exams set that have collisions with any of the values in the inserts set.
     * It then checks the extracted exams for their connected exams and inserts them into the return set.
     * The function returns the set of extracted exams.
     *
     * @param inserts The set of integers representing the inserts.
     * @param exams The set of integers representing the exams.
     * @return The set of exams that have collisions with the inserts and are connected to the extracted exams.
     */
    std::set<int> extractCollisionsAndConnected(const std::set<int>& inserts, std::set<int> &exams);

    /**
     * @brief Retrieves the availability of rooms for a given period, excluding specified exams.
     *
     * This function returns a vector containing the availability of rooms for a specific period,
     * while excluding certain exams. If the period is -1, an empty vector is returned.
     *
     * @param period The period for which to retrieve room availability.
     * @param without A set of exams whose rooms to vacate.
     * @return A vector representing the availability of rooms. The value 0 represents an available room,
     * the value 1 represents an unavailable room and the value -1 represents an invalid room.
     */
    std::vector<int> getPeriodRoomsAvailabilityWithout(const int &period, const std::set<int> &without);

    /**
     * Calculates the absolute period on the same day, given a base period and a relative period.
     *
     * This function determines the period by adding the `relativePeriod` to the `basePeriod`.
     * It checks whether the resulting period exists within the valid range of periods and
     * ensures that the computed period falls on the same day as the `basePeriod`.
     *
     * If the `relativePeriod` is zero, the function returns the `basePeriod`.
     * If the `basePeriod` is out of the valid range or the computed period does not fall
     * on the same day as the `basePeriod`, the function returns an empty optional.
     *
     * @param basePeriod The starting period, used as the reference point.
     * @param relativePeriod The offset to be added to the `basePeriod`.
     * @return An optional containing the resulting absolute period on the same day,
     *         or an empty optional if the resulting period is invalid or not on the same day
     *         as the `basePeriod`.
     */
    std::optional<int> getAbsolutePeriodSameDay(const int basePeriod, const int relativePeriod);

};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXAMTTSOLUTIONMANIPULATOR_H

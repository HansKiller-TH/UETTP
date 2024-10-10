//
// Created by hansk on 12.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCHC_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCHC_H

#include <iostream>
#include <random>
#include <memory>
#include <functional>
#include <utility>
#include "data_utils/ExamTTData.h"
#include "ExamTTSolutionManipulator.h"
#include "CostLogger.h"

/** Step-Counting-Hill-Climbing-Algorithm improves an initial solution_ with perturbative heuristics */
class SCHC {
public:
    explicit SCHC(const std::shared_ptr<ExamTTSolution>& solution) : gen_(std::random_device{}()) {
        std::random_device rd;
        gen_.seed(rd());
        currentSolution_ = std::make_shared<ExamTTSolution>(*solution);
        manipulator_ = std::make_shared<ExamTTSolutionManipulator>(solution);
    }
    /** Executes the algorithm. Returns a shared_ptr to the last ExamTTData object with the best solution_*/
    std::shared_ptr<ExamTTSolution> run();
    /** Returns the variation of the algorithm what steps to count, counterLimit and strategy for room assignment*/
    [[nodiscard]] std::string getConfig() const;
    /** Counts all feasible steps */
    bool schc_all = true;
    /** Counts all accepted steps */
    bool schc_acp = false;
    /** Counts all improving steps */
    bool schc_imp = false;
    /** assign random or best fitting rooms */
    bool random = true;
    int randomSampleSize = 0;
    /** Calculate cost with full collision matrix or collisions limited to students with enrollment of max 7 exams.
     * Hard constraints are unaffected*/
    bool fullCollisionCost = false;
    /** The algorithm will stop after this period of time. If value <= 0.0 it will run until it converges */
    double stopTime = 0.0;
    /** After this many steps the bound is set to current cost */
    int counterLimit = 2000;
    /** Prevents premature termination a the beginning*/
    int minStepsWithoutImprovement = 100;
    /** steps with out improvement must be lower than total steps divided by fraction. 100 is 1%*/
    int fraction = 100;
    /** cost and time */
    std::vector<std::pair<int,double>> valueLog;
    std::shared_ptr<ExamTTSolution> currentSolution_;
private:
    std::shared_ptr<ExamTTSolutionManipulator> manipulator_;
    std::mt19937 gen_;
    int stepsWithoutImprovement = 0;
    int totalSteps = 0;
    std::chrono::high_resolution_clock::time_point start;

    /**
     * Generates a new candidate solution by performing one of several types of moves.
     *
     * This function uses a random number generator to select one of four move operations (roomMove, shiftMove, swapMove, or slotMove).
     * Each move type attempts to improve the current solution by modifying the room or period assignments of exams.
     * - If the random number is 0, a roomMove is attempted.
     * - If the random number is 1, a shiftMove is attempted.
     * - If the random number is 2, a swapMove is attempted.
     * - If the random number is 3, a slotMove is attempted.
     *
     * If the random sample size is 1, the minimum value for the random distribution is set to 1, ensuring that roomMove is not selected.
     * This is used when specific configurations require direct exam assignments to fitting rooms without room moves.
     *
     * @return true if the selected move operation successfully generates a new candidate solution, false otherwise.
     * @throws std::runtime_error if an invalid random number is generated.
     */
    bool createCandidateSolution();

    /**
     * Attempts to move an exam to a different room within the same period.
     *
     * This function randomly selects an exam and tries to reassign its room while keeping it in the same period.
     * It utilizes the `ExamTTSolutionManipulator` to perform the reassignment based on a sample size defined by `randomSampleSize`.
     *
     * @return true if the room reassignment is successful, false otherwise.
     */
    bool roomMove();

    /**
     * Attempts to shift a randomly chosen exam from one period to another randomly chosen period and resolve any resulting conflicts.
     *
     * This function selects a random exam and its period, then attempts to move it to another randomly chosen period.
     * If the two periods are the same, the function immediately returns false. Otherwise, it evaluates the feasibility
     * of the move by creating Kempe chains for the exams in each period, and attempts the shift using the `trySwap` method.
     *
     * @return true if the shift and conflict resolution was successful, false otherwise.
     */
    bool shiftMove();

    /**
     * Attempts to swap the periods of two randomly chosen exams.
     *
     * This function selects two random exams from different periods and attempts
     * to swap their periods. It uses Kempe chains for conflict resolution and
     * the `trySwap` method to evaluate and perform the swap.
     *
     * @return true if the swap was successful, false otherwise.
     */
    bool swapMove();

    /**
     * Attempts to swap the exams between two randomly selected periods.
     *
     * The function randomly selects two different periods and retrieves the corresponding exams.
     * It then assesses the feasibility of the swap, checking for period change infeasibilities.
     * If all checks pass, it attempts to switch the rooms used by the exams.
     * Finally, it moves the exams between the periods if the room switch is successful.
     *
     * @return true if the period swap is successful, false otherwise.
     */
    bool slotMove();

    /**
     * Determines if a proposed period change is infeasible.
     *
     * This function evaluates whether a period change for a set of exams would result in any invalid states.
     * The infeasibility check includes:
     * - Checking if the period move is empty or the period is unassigned.
     * - Verifying if any exam becomes invalid in the proposed period.
     * - Checking for exam collisions with adjacent periods on the same day.
     *
     * @param change The proposed period change consisting of the period to move to and the exams to be moved.
     * @return true if the period change is infeasible, false otherwise.
     */
    bool isPeriodChangeInfeasible(const PeriodChange &change);

    /**
     * Attempts to swap the periods of two given sets of exams.
     *
     * This method evaluates feasibility of the proposed period changes for two given sets of exams.
     * If the changes are feasible and the room assignments can be made, it performs the swap.
     *
      * @param first The first proposed period change with the exams to be moved.
     * @param second The second proposed period change with the exams to be moved.
     * @return true if the swap was successful, false otherwise.
     */
    bool
    trySwap(const PeriodChange &first, const PeriodChange &second);

    /**
     * Checks if the algorithm has not yet converged.
     *
     * This function determines whether the current optimization process should be continued.
     * Convergence is evaluated based on the number of steps taken without improvement. The
     * algorithm is considered to have not converged if the number of steps without improvement
     * is less than or equal to either `minStepsWithoutImprovement` or a fraction of the total steps.
     *
     * @return true if the algorithm should continue running, false if it has converged.
     */
    [[nodiscard]] bool notConverged() const;

    /**
     * Checks if the allocated run time for the algorithm has elapsed.
     *
     * This function calculates the elapsed time since the algorithm started and
     * compares it to the predefined stopping time (`stopTime`). If the elapsed
     * time is greater than or equal to `stopTime`, the function returns true,
     * indicating that the time is up and the algorithm should stop. If the
     * elapsed time is less than `stopTime`, it returns false, allowing the
     * algorithm to continue running.
     *
     * @return true if the elapsed time is greater than or equal to `stopTime`,
     *         false otherwise.
     */
    bool timesUp();
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCHC_H

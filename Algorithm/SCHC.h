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

    /** A neighbourhood solution_ is created. Returns true if feasible*/
    bool createCandidateSolution();

    /** a random exam is moved to random rooms within its period or all rooms in a period are reassigned best fit */
    bool roomMove();

    /** a random exam is moved to a random period. Collisions within the period(s) are resolved with kempe chain */
    bool shiftMove();

    /** Two random exams are moved to the other's period. Collisions within the period(s) are resolved with kempe chain */
    bool swapMove();

    /** The exams of two random periods are switched with their rooms */
    bool slotMove();

    bool isAnyExamInfeasibleInPeriodExcludingCollisionWithIt(const std::set<int> &exams, const int &period);

    bool
    trySwap(const RoomAssignment &first, const RoomAssignment &second);

    [[nodiscard]] bool notConverged() const;

    bool timesUp();
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_SCHC_H

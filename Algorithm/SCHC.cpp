//
// Created by hansk on 12.05.2024.
//

#include <stdexcept>
#include <algorithm>
#include "SCHC.h"
#include "screenOutput.h"
#include "Evaluation.h"
#include "CostLogger.h"

std::shared_ptr<ExamTTSolution> SCHC::run() {
    std::cout << "Running Step Counting Hill Climbing Algorithm..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    //std::cout << (Evaluation::isFeasible(currentSolution_) ? "feasible" : "infeasible") << std::endl;
    //screenOutput::solutionOut(currentSolution_);
    int currentCost = 0;
    if(fullCollisionCost)
        currentCost = Evaluation::calculateCost(*currentSolution_);
    else
        currentCost = Evaluation::calculateAltCost(*currentSolution_);
    int costBound = currentCost;
    // costAndTimeLog.emplace_back(costBound,0.0);
    int stepCounter = 0;
    stepsWithoutImprovement = 0;
    totalSteps = 0;
    while ((stopTime <= 0.0 && notConverged()) || (stopTime > 0.0 && !timesUp())) {
        if (!createCandidateSolution())
            continue;
        auto candidate = manipulator_->getSolution();
        int candidateCost = 0;
        if(fullCollisionCost)
            candidateCost = Evaluation::calculateCost(*candidate);
        else
            candidateCost = Evaluation::calculateAltCost(*candidate);
        if (candidateCost < costBound || candidateCost <= currentCost) {
            if (candidateCost < currentCost) {
                stepsWithoutImprovement = 0;
                if (schc_imp)
                    ++stepCounter; // variant count improving
            } else {
                ++stepsWithoutImprovement;
            }
            currentCost = candidateCost;
            currentSolution_ = std::make_shared<ExamTTSolution>(*candidate);
            if (schc_acp)
                ++stepCounter; // variant count accepting
        } else {
            manipulator_->setSolution(currentSolution_);
            ++stepsWithoutImprovement;
        }
        if (schc_all)
            ++stepCounter; // variant count all
        if (stepCounter >= counterLimit) {
            costBound = currentCost;
            stepCounter = 0;
            // auto now = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> duration = now - start;
            // costAndTimeLog.emplace_back(costBound,duration.count());
            //std::cout << std::to_string(currentCost) << std::endl;
        }
        ++totalSteps;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Config: " << getConfig() << std::endl;
    std::cout << (Evaluation::isFeasible(*currentSolution_) ? "feasible" : "infeasible") << std::endl;
    std::cout << "Cost: " << currentCost << " Time: " << duration.count() << std::endl;
    std::cout << "Total Steps: " << std::to_string(totalSteps) << " Idle Steps: "
              << std::to_string(stepsWithoutImprovement) << std::endl;
    currentSolution_->cost = currentCost;
    currentSolution_->runTime = duration.count();
    currentSolution_->creationDateTime = DateTimeProvider::getDateTimeNow();
    currentSolution_->costLimit = Evaluation::spreadCostLimit(*currentSolution_);
    currentSolution_->costAbove = Evaluation::spreadCostBeyond(*currentSolution_);
    currentSolution_->configuration = getConfig();
    // currentSolution_.costAndTimeLog = costAndTimeLog;
    return currentSolution_;
}

std::string SCHC::getConfig() const {
    std::string result = currentSolution_->examData->dataSet + "_schc_";
    if (schc_all)
        result += "all_";
    if (schc_acp)
        result += "acp_";
    if (schc_imp)
        result += "imp_";
    result += randomSampleSize == 1 ? "fit_" : "rand_";
    if(!fullCollisionCost)
        result += "limit_";
    result += "Lc" + std::to_string(counterLimit) + "_";
    if(stopTime > 0.0)
        result += "stopTime" + std::to_string(static_cast<int>(stopTime)) + "_";
    return result;
}

bool SCHC::createCandidateSolution() {
    // if random is false we want to assign fitting rooms to the exams and do not need the roomMove
    int min = randomSampleSize == 1 ? 1 : 0;
    std::uniform_int_distribution<> distr(min, 3);
    int rand = distr(gen_);
    switch (rand) {
        case 0: {
            return roomMove();
        }
        case 1: {
            return shiftMove();
        }
        case 2: {
            return swapMove();
        }
        case 3: {
            return slotMove();
        }
        default:
            throw std::runtime_error("Invalid random number generated");
    }
}

bool SCHC::roomMove() {
    auto randExam = manipulator_->getRandomExam();
    return manipulator_->tryReassignRoomsToExamSamePeriod(randomSampleSize, randExam);
}

bool SCHC::shiftMove() {
    auto randFirst = manipulator_->getRandomExamWithConnectedWithPeriod();
    auto displacedFirst = randFirst.first;
    auto periodFirst = randFirst.second;
    auto periodSecond = manipulator_->getRandomPeriod();
    if (periodFirst == periodSecond)
        return false;

    std::set<int> displacedSecond;
    auto examsFirst = manipulator_->getExamsInPeriodWithout(displacedFirst);
    auto examsSecond = manipulator_->getExamsInPeriod(periodSecond);

    manipulator_->kempeChain(displacedFirst, displacedSecond, examsFirst, examsSecond);

    return trySwap({periodFirst, displacedSecond}, {periodSecond, displacedFirst});
}

bool SCHC::swapMove() {
    auto randFirst = manipulator_->getRandomExamWithConnectedWithPeriod();
    auto displacedFirst = randFirst.first;
    auto periodFirst = randFirst.second;
    auto randSecond = manipulator_->getRandomExamWithConnectedWithPeriod();
    auto displacedSecond = randSecond.first;
    auto periodSecond = randSecond.second;

    if (periodFirst == periodSecond)
        return false;

    auto examsFirst = manipulator_->getExamsInPeriodWithout(displacedFirst);
    auto examsSecond = manipulator_->getExamsInPeriodWithout(displacedSecond);

    manipulator_->kempeChain(displacedFirst, displacedSecond, examsFirst, examsSecond);

    return trySwap({periodFirst, displacedSecond}, {periodSecond, displacedFirst});
}

bool SCHC::slotMove() {
    auto periodFirst = manipulator_->getRandomPeriod();
    auto periodSecond = manipulator_->getRandomPeriod();
    if (periodFirst == periodSecond)
        return false;
    auto examsFirst = manipulator_->getExamsInPeriod(periodFirst);
    auto examsSecond = manipulator_->getExamsInPeriod(periodSecond);
    if (examsFirst.empty() && examsSecond.empty())
        return false;
    PeriodChange first(periodFirst,examsSecond);
    PeriodChange second(periodSecond, examsFirst);

    if (isPeriodChangeInfeasible(first))
        return false;
    if (isPeriodChangeInfeasible(second))
        return false;

    if (!manipulator_->trySwitchUsedRooms(first, second))
        return false;

    manipulator_->moveExamsToPeriod(first, second);
    return true;
}

bool SCHC::trySwap(const PeriodChange &first, const PeriodChange &second) {
    if (isPeriodChangeInfeasible(first))
        return false;
    if (isPeriodChangeInfeasible(second))
        return false;
    if (!manipulator_->tryAssignRandomRooms(randomSampleSize, first, second))
        return false;
    manipulator_->moveExamsToPeriod(first, second);
    return true;
}

bool SCHC::isPeriodChangeInfeasible(const PeriodChange &change) {
    if (change.moveIn.empty() || change.period == -1)
        return false;
    if (manipulator_->isAnyExamInvalidInPeriod(change.moveIn, change.period))
        return true;
    if (manipulator_->hasAnyExamCollisionWithAnyPeriod(change, {-1, 1}))
        return true;
    return false;
}

bool SCHC::notConverged() const {
    return stepsWithoutImprovement <= minStepsWithoutImprovement || stepsWithoutImprovement <= totalSteps / fraction;
}

bool SCHC::timesUp() {
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    return duration.count() >= stopTime;
}

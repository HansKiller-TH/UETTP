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
    result += random ? "rand_" : "fit_";
    if(!fullCollisionCost)
        result += "limit_";
    result += "Lc" + std::to_string(counterLimit) + "_";
    if(stopTime > 0.0)
        result += "stopTime" + std::to_string(static_cast<int>(stopTime)) + "_";
    return result;
}

bool SCHC::createCandidateSolution() {
    // if random is false we want to assign fitting rooms to the exams and do not need the roomMove
    int min = random ? 0 : 1;
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
    int randomSampleSize = random ? 0 : 1;
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
    PeriodChange first(periodFirst, displacedSecond, displacedFirst);
    PeriodChange second(periodSecond, displacedFirst, displacedSecond);
    return trySwap(first, second);
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
    PeriodChange first(periodFirst, displacedSecond, displacedFirst);
    PeriodChange second(periodSecond, displacedFirst, displacedSecond);
    return trySwap(first, second);
}

bool SCHC::slotMove() {
    auto periodFirst = manipulator_->getRandomPeriod();
    auto periodSecond = manipulator_->getRandomPeriod();
    if (periodFirst == periodSecond)
        return false;
    if (manipulator_->getExamsInPeriod(periodFirst).empty() && manipulator_->getExamsInPeriod(periodSecond).empty())
        return false;
    auto examsFirst = manipulator_->getExamsInPeriod(periodFirst);
    if (isAnyExamInfeasibleInPeriodExcludingCollisionWithIt(examsFirst, periodSecond))
        return false;

    auto examsSecond = manipulator_->getExamsInPeriod(periodSecond);
    if (isAnyExamInfeasibleInPeriodExcludingCollisionWithIt(examsSecond, periodFirst))
        return false;

    if (!manipulator_->trySwitchUsedRooms(periodFirst, periodSecond))
        return false;

    manipulator_->moveExamsToPeriod(examsFirst, periodSecond);
    manipulator_->moveExamsToPeriod(examsSecond, periodFirst);
    return true;
}

bool SCHC::trySwap(const PeriodChange &first, const PeriodChange &second) {
    if (isAnyExamInfeasibleInPeriodExcludingCollisionWithIt(first.moveIn, first.period))
        return false;
    if (isAnyExamInfeasibleInPeriodExcludingCollisionWithIt(second.moveIn, second.period))
        return false;
    int randomSampleSize = random ? 0 : 1;
    if (!manipulator_->tryAssignRandomRoomsForEachExamInOtherPeriod(randomSampleSize, first, second))
        return false;
    manipulator_->moveExamsToPeriod(second.moveIn, second.period);
    manipulator_->moveExamsToPeriod(first.moveIn, first.period);
    return true;
}

bool SCHC::isAnyExamInfeasibleInPeriodExcludingCollisionWithIt(const std::set<int> &exams, const int &period) {
    if (exams.empty() || period == -1)
        return false;
    if (manipulator_->isAnyExamInvalidInPeriod(exams, period))
        return true;
    auto previous = manipulator_->getPreviousPeriodSameDay(period);
    if (manipulator_->hasAnyExamCollisionWithPeriod(exams, previous))
        return true;
    auto next = manipulator_->getNextPeriodSameDay(period);
    if (manipulator_->hasAnyExamCollisionWithPeriod(exams, next))
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

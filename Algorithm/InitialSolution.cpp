//
// Created by hansk on 29.04.2024.
//

#include <set>
#include <algorithm>
#include "InitialSolution.h"

std::set<int> InitialSolution::getNextExam() {
    return manipulator->removeExamDSatur(unscheduledExams);
}

bool InitialSolution::scheduleExam(const std::set<int> &exams) {
    for (auto period: manipulator->getValidPeriodsForExams(exams)) {
        auto previous = manipulator->getPreviousPeriodSameDay(period);
        auto next = manipulator->getNextPeriodSameDay(period);
        if (manipulator->hasAnyExamCollisionWithAnyPeriod(exams, {previous, period, next}))
            continue;
        PeriodChange change(period, exams);
        if (!manipulator->tryAssignRandomRooms(randomSampleSize, change))
            continue;
        manipulator->moveExamsToPeriod(change);
        return true;
    }
    std::cout << "No slot available for exams:";
    for (const auto& item:exams)
        std::cout << " " << std::to_string(item);
    std::cout << std::endl;
    return false;
}


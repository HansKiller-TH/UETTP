//
// Created by hansk on 29.04.2024.
//

#include <set>
#include <algorithm>
#include "InitialSolution.h"

std::set<int> InitialSolution::getNextExam() {
    /*if (unscheduledExams.empty())
        return {};
    if (largeExams) {
        auto exams = manipulator->removeLargestExam(unscheduledExams);
        if (!exams.empty())
            return exams;
        largeExams = false;
    }
    return manipulator->removeExamWithLargestDegreeOfSaturation(unscheduledExams);*/
    return manipulator->removeExamDSatur(unscheduledExams);
}

bool InitialSolution::scheduleExam(const std::set<int> &exams) {
    if (exams.empty()) {
        std::cout << "finished initial schedule" << std::endl;
        return false;
    }
    for (auto period: manipulator->getValidPeriodsForExams(exams)) {
        PeriodChange change(period, exams);
        if (manipulator->hasAnyExamCollisionWithPeriod(exams, manipulator->getPreviousPeriodSameDay(period)))
            continue;
        if (manipulator->hasAnyExamCollisionWithPeriod(exams, period))
            continue;
        if (manipulator->hasAnyExamCollisionWithPeriod(exams, manipulator->getNextPeriodSameDay(period)))
            continue;
        if (!manipulator->tryAssignRandomRoomsForEachExamInOtherPeriod(randomSampleSize, change))
            continue;
        manipulator->moveExamsToPeriod(exams, period);
        return true;
    }
    std::string examsStr;
    std::for_each(exams.begin(), exams.end(), [&](const int &value) { examsStr += std::to_string(value) + " "; });
    std::cout << "No slot available for exams: " << examsStr << std::endl;
    return false;
}


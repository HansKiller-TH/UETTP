//
// Created by hansk on 06.10.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_PERIODCHANGE_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_PERIODCHANGE_H


#include <set>

struct PeriodChange {
    const int period;
    const std::set<int> moveIn;

    PeriodChange(const int period, const std::set<int> &moveIn) : period(period), moveIn(moveIn) {}

    explicit PeriodChange(const int period) : period(period){}

    PeriodChange() : period(-1) {}
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_PERIODCHANGE_H

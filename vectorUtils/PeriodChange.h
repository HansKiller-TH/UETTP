//
// Created by hansk on 06.10.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_PERIODCHANGE_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_PERIODCHANGE_H


#include <set>

struct PeriodChange {
    const int period;
    const std::set<int> moveIn;
    const std::set<int> moveOut;
    explicit PeriodChange(const int period, const std::set<int>& in, const std::set<int>& out) : period(period), moveIn(in), moveOut(out) {}
    explicit PeriodChange(const int period, const std::set<int>& in) : period(period), moveIn(in) {}
    PeriodChange() : period(-1) {}
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_PERIODCHANGE_H

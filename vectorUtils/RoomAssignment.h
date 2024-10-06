//
// Created by hansk on 06.10.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ROOMASSIGNMENT_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ROOMASSIGNMENT_H


#include <set>

struct RoomAssignment {
    const int period;
    const std::set<int> assignRooms;
    const std::set<int> vacateRooms;
    explicit RoomAssignment(const int period, const std::set<int>& assign, const std::set<int>& vacate) : period(period), assignRooms(assign), vacateRooms(vacate) {}
    explicit RoomAssignment(const int period, const std::set<int>& assign) : period(period), assignRooms(assign) {}
    RoomAssignment() : period(-1) {}
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ROOMASSIGNMENT_H

//
// Created by hansk on 03.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXCEPTIONS_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXCEPTIONS_H
#include <stdexcept>
#include <string>

class DatabaseQueryException : public std::runtime_error {
public:
    explicit DatabaseQueryException(const std::string &message)
            : std::runtime_error(message) {}
};
#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_EXCEPTIONS_H

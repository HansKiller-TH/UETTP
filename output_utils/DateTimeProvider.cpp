//
// Created by hansk on 10.07.2024.
//

#include <chrono>
#include <iomanip>
#include "DateTimeProvider.h"
std::string DateTimeProvider::getDateTimeNow() {
    auto now = std::chrono::system_clock::now();

    // Convert to time_t for time formatting
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert to tm structure for local time
    std::tm now_tm = *std::localtime(&now_time_t);

    // Create a stringstream to format the date and time
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y_%m_%d_%H_%M_%S");

    // Return the formatted string
    return ss.str();
}
#include "utils.h"

static std::chrono::high_resolution_clock::time_point startTime;

void Utils::startTimer() {
    startTime = std::chrono::high_resolution_clock::now();
}

double Utils::stopTimer() {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    return duration.count();
}

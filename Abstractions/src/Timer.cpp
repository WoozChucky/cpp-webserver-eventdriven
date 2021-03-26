//
// Created by Nuno on 3/26/2021.
//

#include <Abstractions/Timer.hpp>
#include <stdexcept>

Timer::Timer() {
    this->_started = true;
    this->_start = std::chrono::high_resolution_clock::now();
}

Timer::Timer(bool start) {
    this->_started = start;
    if (start) {
        this->_start = std::chrono::high_resolution_clock::now();
    }
}

void Timer::Start() {
    this->_start = std::chrono::high_resolution_clock::now();
}

U64 Timer::Stop() {

    if (!this->_started)
        throw std::runtime_error("Timer has not been started");

    auto end = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(this->_start)
            .time_since_epoch()
            .count();
    auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(end)
            .time_since_epoch()
            .count();

    return stop - start;
}




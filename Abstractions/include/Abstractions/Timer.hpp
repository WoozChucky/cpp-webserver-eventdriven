//
// Created by Nuno on 3/26/2021.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include <Abstractions/Types.hpp>
#include <chrono>

/**
 * @brief Timer class that measures elapsed time in microseconds.
 *
 * @note 1 microsecond = 0.001 milliseconds
 * @note 1 millisecond = 1000 microseconds
 */
class Timer {

public:

    /**
     * @brief Creates a new Timer instance immediately starting the timer.
     */
    explicit Timer();

    /**
     * @brief  Creates a new Timer instance
     * @param start Whether or not to immediately start the timer
     */
    explicit Timer(bool start);

    /**
     * @brief Starts or Restarts the timer if already running
     */
    void Start();

    /**
     * @brief Stops the timer
     * @throws std::runtime_error If timer hasn't been started
     * @return number of microseconds elapsed
     */
    U64 Stop();

private:

    std::chrono::time_point<std::chrono::high_resolution_clock> _start;

    bool _started = false;

};

#endif //TIMER_HPP

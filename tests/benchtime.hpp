// benchtime.hpp

#ifndef LEVENSHTEIN_TESTS_BENCHTIME_HPP_
#define LEVENSHTEIN_TESTS_BENCHTIME_HPP_

#include <iostream>
#include <chrono>

class Timer {
public:
    Timer() : running(false) {}

    // Starts or restarts the timer
    void start() {
        start_time = clock_::now();
        running = true;
    }

    // Stops the timer
    void stop() {
        end_time = clock_::now();
        running = false;
    }

    // Returns the elapsed time in seconds
    double elapsed() const {
        if (running) {
            return std::chrono::duration_cast<second_>(clock_::now() - start_time).count();
        } else {
            return std::chrono::duration_cast<second_>(end_time - start_time).count();
        }
    }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> start_time;
    std::chrono::time_point<clock_> end_time;
    bool running;
};

#endif //LEVENSHTEIN_TESTS_BENCHTIME_HPP_

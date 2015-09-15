#ifndef TIMER_H
#define TIMER_H

#include "utilities.h"
#if OPERATING_SYSTEM == WINDOWS
#include "utilities_windows.h"
#endif

#include <iosfwd>


class Timer {
    double last_start_clock;
    double collected_time;
    bool stopped;
#if OPERATING_SYSTEM == WINDOWS
    LARGE_INTEGER frequency;
    LARGE_INTEGER start_ticks;
#endif

    double current_clock() const;
public:
    Timer();
    ~Timer();
    double operator()() const;
    double stop();
    void resume();
    double reset();
};

std::ostream &operator<<(std::ostream &os, const Timer &timer);

#endif

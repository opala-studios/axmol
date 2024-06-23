#include "OPTimer.hpp"

void OPTimer::start()
{
    if (running) { return; }
    running = true;

    startTime = std::chrono::system_clock::now();
    finishTime = startTime;
}

void OPTimer::restart()
{
    running = false;
    start();
}

double OPTimer::finish()
{
    if (!running) { return 0; }
    running = false;

    finishTime = std::chrono::system_clock::now();
    return getElapsedTime();
}

double OPTimer::getElapsedTime()
{
    auto current = running ? std::chrono::system_clock::now() : finishTime;
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(current - startTime);
    return duration.count();
}

double OPTimer::getElapsedTimeMilliSec()
{
    return getElapsedTime()*1000.0;
}

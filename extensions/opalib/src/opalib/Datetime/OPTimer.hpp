#ifndef OPTimer_hpp
#define OPTimer_hpp

#include <chrono>
#include <unistd.h>

class OPTimer
{
private:

    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point finishTime;

    bool running {false};

public:

    void start();
    double finish();

    void restart();

    bool isRunning() { return running; }
    double getElapsedTime();
    double getElapsedTimeMilliSec();

};

#endif

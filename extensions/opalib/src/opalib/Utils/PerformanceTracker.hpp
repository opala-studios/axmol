//
// Created by Ricardo on 02/12/20.
//
//Classe de utilidade para detectar quedas no framerate

#ifndef IT_PERFORMANCETRACKER_HPP
#define IT_PERFORMANCETRACKER_HPP

#include <vector>
#include <chrono>

namespace opalib {

    class PerformanceTracker {
    public:
        struct Result {
            int recordCount;
            float accumulatedDuration;
            float avgFps;
        };
    private:
        struct Record {
            float duration;
            float avgFps;
        };

        struct Accumulator {
            float time = 0;
            int frames = 0;
        };

    public:
        PerformanceTracker(int framesPerRecordingInterval, float minimumTargetFps);

        void update(float dt);

        Result getResult();

    private:
        void record();

    private:
        const int _framesPerRecordingInterval;
        const float _minimumTargetFps;
        int _frameAccumulator = 0;
        float _timeAccumulator = 0;
        std::unique_ptr<Accumulator> _recordAccumulator;
        std::vector<Record> _records;
    };
}

#endif //IT_PERFORMANCETRACKER_HPP

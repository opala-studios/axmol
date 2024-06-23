//
// Created by Opala Teste on 02/12/20.
//


#include "PerformanceTracker.hpp"

namespace opalib {

    PerformanceTracker::PerformanceTracker(int framesPerRecordingInterval, float minimumTargetFps) :
            _framesPerRecordingInterval(framesPerRecordingInterval),
            _minimumTargetFps(minimumTargetFps) {

    }

    void PerformanceTracker::update(float dt) {
        _frameAccumulator++;
        _timeAccumulator += dt;
        if (_frameAccumulator >= _framesPerRecordingInterval){
            float avgDt = _timeAccumulator / static_cast<float>(_frameAccumulator);
            float avgFps = 1.0f / avgDt;
            if (avgFps < _minimumTargetFps){
                if (!_recordAccumulator){
                    _recordAccumulator.reset(new Accumulator());
                }
                _recordAccumulator->frames += _frameAccumulator;
                _recordAccumulator->time += _timeAccumulator;
            }
            else if (_recordAccumulator){
                record();
            }
            _frameAccumulator = 0;
            _timeAccumulator = 0;
        }
    }

    void PerformanceTracker::record() {
        Record record = {};
        record.duration = _recordAccumulator->time;
        record.avgFps = static_cast<float>(_recordAccumulator->frames) / record.duration;
        _records.emplace_back(record);
        _recordAccumulator.reset();
    }

    PerformanceTracker::Result PerformanceTracker::getResult() {
        if (_recordAccumulator){
            record();
        }

        Result result = {};
        result.recordCount = _records.size();
        if (result.recordCount > 0) {
            float avgFps = 0;
            for (auto &record : _records) {
                avgFps += record.avgFps;
                result.accumulatedDuration += record.duration;
            }
            result.avgFps = avgFps / static_cast<float>(result.recordCount);
        }
        else {
            result.accumulatedDuration = 0;
            result.avgFps = 60.0f;
        }
        return result;
    }
}
//
// Created by bruno on 31/05/2021.
//

#include <Utils/OPCollections.h>

#include <Playables/OPPlayable.hpp>
#include <algorithm>
#include <utility>

bool OPSequence::isWithin(const OPFrameTimes& times) const {
    return isWithin(times.time);
}

bool OPSequence::isWithin(const float time) const {
    return startTime <= time && endTime >= time;
}

float OPSequence::getStartTime() const {
    return startTime;
}

float OPSequence::getEndTime() const {
    return endTime;
}

const std::vector<std::shared_ptr<OPPlayable>>& OPSequence::getElements() const {
    return elements;
}

void OPSequence::evaluate(OPFrameTimes times) {
    for (std::shared_ptr<OPPlayable>& item : elements) {
        item->evaluate(times);
    }
}

void OPSequence::begin(OPFrameTimes times) {
    for (std::shared_ptr<OPPlayable>& item : elements) {
        item->begin(times);
    }
}

void OPSequence::end(OPFrameTimes times) {
    for (std::shared_ptr<OPPlayable>& item : elements) {
        item->end(times);
    }
}
OPSequence::OPSequence(std::string sequenceName, float startTime, float endTime) :
    name(std::move(sequenceName)),
    startTime(startTime),
    endTime(endTime) {}
OPSequence::OPSequence(float startTime, float endTime) :
    OPSequence("Unnamed Sequence", startTime, endTime) {}

void OPSequence::addPlayable(const std::shared_ptr<OPPlayable>& playable) { elements.push_back(playable); }
const std::string& OPSequence::getName() const {
    return name;
}

float OPTimeline::getStartTime() const {
    if (_sequences.empty()) {
        return 0;
    }
    float earliest = std::numeric_limits<float>::max();
    for (const OPSequence& sequence : _sequences) {
        float startTime = sequence.getStartTime();
        if (earliest > startTime) {
            earliest = startTime;
        }
    }
    for (const OPMarker& marker : _markers) {
        float startTime = marker.getTime();
        if (earliest > startTime) {
            earliest = startTime;
        }
    }
    return earliest;
}

float OPTimeline::getEndTime() const {
    if (_sequences.empty()) {
        return 0;
    }
    float latest = std::numeric_limits<float>::min();
    for (const OPSequence& sequence : _sequences) {
        float startTime = sequence.getEndTime();
        if (latest < startTime) {
            latest = startTime;
        }
    }
    for (const OPMarker& marker : _markers) {
        float startTime = marker.getTime();
        if (latest < startTime) {
            latest = startTime;
        }
    }
    return latest;
}

void OPTimeline::enqueue(OPSequence&& sequence) {
    _sequences.push_back(std::move(sequence));
}

void OPTimeline::update(float deltaTime) {
    evaluate(_currentTime + deltaTime);
}

void OPTimeline::evaluate(float time) {
    float oldTime = _currentTime;
    _currentTime = time;
    executeSequences();
    executeMarkers(oldTime);
}

void OPTimeline::executeSequences() {
    size_t end = _sequences.size();
    for (size_t i = 0; i < end; ++i) {
        OPSequence& sequence = _sequences[i];
        bool within = sequence.isWithin(_currentTime);
        OPFrameTimes relative{};
        relative.time = _currentTime;
        relative.progress = OPMath::inv_lerp(
            sequence.getStartTime(),
            sequence.getEndTime(),
            _currentTime
        );
        if (within) {
            if (!OPCollections::contains(_lastActiveSequences, i)) {
                sequence.begin(relative);
                _lastActiveSequences.emplace(i);
            }
        } else {
            if (OPCollections::contains(_lastActiveSequences, i)) {
                sequence.end(relative);
                _lastActiveSequences.erase(i);
            }
            continue;
        }
        sequence.evaluate(relative);
    }
}
void OPTimeline::executeMarkers(float oldTime) {
    bool isForward = _currentTime >= oldTime;
    for (const OPMarker& marker : _markers) {
        if (marker.shouldFire(_currentTime, oldTime)) {
            marker.fire(oldTime, _currentTime);
        }
        float markerTime = marker.getTime();
        if (isForward) {
            if (markerTime > _currentTime) {
                //Early exit
                break;
            }
        } else {
            if (markerTime < _currentTime) {
                //Early exit
                break;
            }
        }
    }
}
void OPTimeline::clear() {
    _sequences.clear();
    _markers.clear();
}
void OPTimeline::addMarker(
    float time,
    const OPMarker::OPMarkerCallback& callback,
    OPMarker::OPMarkerActivationMode mode
) {
    addMarker("Unnamed Marker", time, callback, mode);
}
void OPTimeline::addMarker(const std::string& name, float time, const OPMarker::OPMarkerCallback& callback, OPMarker::OPMarkerActivationMode mode) {
    addMarker(
        name,
        time,
        [callback](float from, float to, OPMarker::OPMarkerActivationMode mode) {
            callback(from, to);
        },
        mode
    );
}

void OPTimeline::addMarker(
    float time,
    const OPMarker::OPMarkerAdvancedCallback& callback,
    OPMarker::OPMarkerActivationMode mode
) {
    _markers.emplace_back(time, callback, mode);
    std::sort(_markers.begin(), _markers.end());
}
void OPTimeline::addMarker(
    const std::string& name,
    float time,
    const OPMarker::OPMarkerAdvancedCallback& callback,
    OPMarker::OPMarkerActivationMode mode
) {

    _markers.emplace_back(name, time, callback, mode);
    std::sort(_markers.begin(), _markers.end());
}
void OPTimeline::reset() {
    _currentTime = getStartTime();
    _lastActiveSequences.clear();
}
const std::vector<OPSequence>& OPTimeline::getSequences() const {
    return _sequences;
}
const std::vector<OPMarker>& OPTimeline::getMarkers() const {
    return _markers;
}

float OPMarker::getTime() const {
    return _time;
}

const OPMarker::OPMarkerAdvancedCallback& OPMarker::getCallback() const {
    return _callback;
}
OPMarker::OPMarker(
    float time,
    OPMarker::OPMarkerAdvancedCallback callback,
    OPMarker::OPMarkerActivationMode activationMode
) :
    OPMarker("Unnamed Marker", time, std::move(callback), activationMode) {}
OPMarker::OPMarker(
    std::string name,
    float time,
    OPMarker::OPMarkerAdvancedCallback callback,
    OPMarker::OPMarkerActivationMode activationMode
) :
    _name(std::move(name)),
    _time(time),
    _callback(std::move(callback)),
    _activationMode(activationMode) {

}
bool OPMarker::shouldFire(float timeNow, float timeBefore) const {
    bool movedForward = timeNow >= timeBefore;

    if (movedForward) {
        if (isForward()) {
            bool lastFrameActive = timeBefore >= _time;
            bool currentFrameActive = timeNow >= _time;
            if (!lastFrameActive && currentFrameActive) {
                return true;
            }
        }
    } else {
        if (isBackward()) {
            bool lastFrameActive = _time <= timeBefore;
            bool currentFrameActive = _time <= timeNow;
            if (!lastFrameActive && currentFrameActive) {
                return true;
            }
        }
    }
    return false;
}
bool OPMarker::isForward() const {
    return (_activationMode & FORWARD) == FORWARD;
}
bool OPMarker::isBackward() const {
    return (_activationMode & BACKWARD) == BACKWARD;
}
void OPMarker::fire(float oldTime, float currentTime) const {
    OPMarkerActivationMode mode;
    if (currentTime >= oldTime) {
        mode = OPMarkerActivationMode::FORWARD;
    } else {
        mode = OPMarkerActivationMode::BACKWARD;
    }
    _callback(oldTime, currentTime, mode);
}
bool OPMarker::operator<(const OPMarker& rhs) const {
    return _time < rhs._time;
}
bool OPMarker::operator>(const OPMarker& rhs) const {
    return rhs < *this;
}
bool OPMarker::operator<=(const OPMarker& rhs) const {
    return !(rhs < *this);
}
bool OPMarker::operator>=(const OPMarker& rhs) const {
    return !(*this < rhs);
}
const std::string& OPMarker::getName() const {
    return _name;
}

float OPFrameTimes::get(OPFrameTimeType type) {
    switch (type) {
        case ABSOLUTE:
            return time;
        case RELATIVE:
            return progress;
    }
}
OPPlayable::OPPlayable() :
    _name("Unnamed Playable") {

}
const std::string& OPPlayable::getName() const {
    return _name;
}
void OPPlayable::setName(const std::string& name) {
    _name = name;
}

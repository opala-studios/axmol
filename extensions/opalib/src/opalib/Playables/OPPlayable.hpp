//
// Created by bruno on 31/05/2021.
//

#ifndef OPALIB_CINEMATICS_OPPLAYABLE_H
#define OPALIB_CINEMATICS_OPPLAYABLE_H

#include <Math/OPMath.hpp>
#include <set>
#include <string>
#include <vector>
#include <memory>

enum OPFrameTimeType {
    RELATIVE,
    ABSOLUTE
};
struct OPFrameTimes {
public:
    float time;
    float progress;
    float get(OPFrameTimeType type);
};

class OPPlayable {
public:
    virtual void begin(OPFrameTimes times) {}
    virtual void end(OPFrameTimes times) {}
    virtual void evaluate(OPFrameTimes times) {};
private:
    std::string _name;
public:
    OPPlayable();
    const std::string& getName() const;
    void setName(const std::string& name);
};
class OPMarker {
public:
    // TODO: Wait for Novak to make OPEvent not cringe so we can use it here
    typedef std::function<void(float oldTime, float currentTime)> OPMarkerCallback;
    enum OPMarkerActivationMode {
        FORWARD = 1 << 0,
        BACKWARD = 1 << 1,
        BOTH = FORWARD | BACKWARD
    };
    typedef std::function<void(float oldTime, float currentTime, OPMarkerActivationMode mode)> OPMarkerAdvancedCallback;

private:
    std::string _name;
    float _time;
    OPMarkerAdvancedCallback _callback;
    OPMarkerActivationMode _activationMode;
public:
    OPMarker(
        std::string name,
        float time,
        OPMarkerAdvancedCallback callback,
        OPMarkerActivationMode activationMode = OPMarkerActivationMode::BOTH
    );
    OPMarker(
        float time,
        OPMarkerAdvancedCallback callback,
        OPMarkerActivationMode activationMode = OPMarkerActivationMode::BOTH
    );
    bool operator<(const OPMarker& rhs) const;
    bool operator>(const OPMarker& rhs) const;
    bool operator<=(const OPMarker& rhs) const;
    bool operator>=(const OPMarker& rhs) const;
    const std::string& getName() const;
    float getTime() const;
    const OPMarkerAdvancedCallback& getCallback() const;
    bool shouldFire(float timeNow, float timeBefore) const;
    bool isForward() const;
    bool isBackward() const;
    void fire(float oldTime, float currentTime) const;
};
class OPSequence : public OPPlayable {
private:
    std::string name;
    float startTime;
    float endTime;
    std::vector<std::shared_ptr<OPPlayable>> elements;

public:
    OPSequence(float startTime, float endTime);
    OPSequence(std::string sequenceName, float startTime, float endTime);
    template<typename T, typename... Args>
    std::shared_ptr<T> addPlayable(Args&& ... args) {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        elements.emplace_back(ptr);
        ptr->setName(typeid(T).name());
        return ptr;
    }
    void addPlayable(const std::shared_ptr<OPPlayable>& playable);
    void begin(OPFrameTimes times) override;
    void end(OPFrameTimes times) override;
    void evaluate(OPFrameTimes times) override;
    float getStartTime() const;
    float getEndTime() const;
    const std::string& getName() const;
    const std::vector<std::shared_ptr<OPPlayable>>& getElements() const;
    bool isWithin(const OPFrameTimes& times) const;
    bool isWithin(const float time) const;
};
class OPTimeline {
private:
    std::vector<OPSequence> _sequences;
    std::vector<OPMarker> _markers;
    std::set<size_t> _lastActiveSequences;
    float _currentTime;

public:
    void enqueue(OPSequence&& sequence);
    void addMarker(
        float time,
        const OPMarker::OPMarkerCallback& callback,
        OPMarker::OPMarkerActivationMode mode = OPMarker::OPMarkerActivationMode::BOTH
    );
    void addMarker(
        float time,
        const OPMarker::OPMarkerAdvancedCallback& callback,
        OPMarker::OPMarkerActivationMode mode = OPMarker::OPMarkerActivationMode::BOTH
    );
    void addMarker(
        const std::string& name,
        float time,
        const OPMarker::OPMarkerCallback& callback,
        OPMarker::OPMarkerActivationMode mode = OPMarker::OPMarkerActivationMode::BOTH
    );
    void addMarker(
        const std::string& name,
        float time,
        const OPMarker::OPMarkerAdvancedCallback& callback,
        OPMarker::OPMarkerActivationMode mode = OPMarker::OPMarkerActivationMode::BOTH
    );
    float getStartTime() const;
    float getEndTime() const;
    void update(float deltaTime);
    void evaluate(float time);
    void clear();
    void reset();
    const std::vector<OPSequence>& getSequences() const;
    const std::vector<OPMarker>& getMarkers() const;
    void executeMarkers(float oldTime);
    void executeSequences();
};
#endif // OPALIB_CINEMATICS_OPPLAYABLE_H

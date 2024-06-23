//
// Created by bruno on 21/07/2021.
//

#ifndef OPAMATH_OPCURVE_HPP
#define OPAMATH_OPCURVE_HPP

#include "OPMath.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <cocos2d.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// So that we don't have to deal with the terrible idea that is cocos ref
template<typename T>
class OPCurve {
    
    /******************************************
     DO NOT MODIFY MAGNITUDE!!! IT'S NOT WORKING
            - magnitude of 1 is fine
     
            * We will improve it soon (BRUNO)
    /******************************************/
    
public:
    static OPCurve constant(T value) {
        OPCurve<T> curve;
        curve.addLinearKeyframe(0, value);
        return curve;
    }

    static OPCurve zero() {
        T zeroValue{};
        return constant(zeroValue);
    }

    static OPCurve linear(T from, T to) {
        OPCurve<T> curve;
        curve.addLinearKeyframe(0, from);
        curve.addLinearKeyframe(1, to);
        return curve;
    }

    static OPCurve easeInOut(T from, T to) {
        OPCurve<T> curve;
        curve.addEaseOutKeyframe(0, from);
        curve.addEaseInKeyframe(1, to);
        return curve;
    }

    static OPCurve easeOut(T from, T to) {
        OPCurve<T> curve;
        curve.addEaseInKeyframe(0, from);
        curve.addFlatKeyframe(1, to);
        return curve;
    }

    static OPCurve easeIn(T from, T to) {
        OPCurve<T> curve;
        curve.addFlatKeyframe(0, from);
        curve.addEaseOutKeyframe(1, to);
        return curve;
    }

    struct OPKeyframe {
        enum OPInterpolation {
            CUBIC,
            HERMITE,
            LINEAR
        };
        float time;
        OPInterpolation interpolation;
        // Both tangents use radians
        union {
            struct {
                float leftTangent;
                float leftTangentMagnitude;
                float rightTangent;
                float rightTangentMagnitude;
            };
        };
        T value;

        bool operator<(const OPKeyframe& rhs) const { return time < rhs.time; }

        bool operator>(const OPKeyframe& rhs) const { return rhs < *this; }

        bool operator<=(const OPKeyframe& rhs) const { return !(rhs < *this); }

        bool operator>=(const OPKeyframe& rhs) const { return !(*this < rhs); }
    };
    static T cubic(float t, OPKeyframe first, OPKeyframe second);
    static T cubic(float t, T p0, T p1, T p2, T p3) {
        // Bézier Curve Cubic Function
        // https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Cubic_B%C3%A9zier_curves
        float oneMinusT = 1 - t;
        float oneMinusTSquared = oneMinusT * oneMinusT;
        float oneMinusTCubed = oneMinusTSquared * oneMinusT;
        float tSquared = t * t;
        float tCubed = tSquared * t;
        return (oneMinusTCubed * p0) + (3 * oneMinusTSquared * t * p1) + (3 * oneMinusT * tSquared * p2) + (tCubed * p3);
    }
    /*
        static T hermite(float time, OPKeyframe first, OPKeyframe second) {
            float leftTime = first.time;
            float rightTime = second.time;
            float dx = rightTime - leftTime;
            float m0;
            float m1;
            float t;
            if (dx != 0.0f) {
                t = (time - leftTime) / dx;
                m0 = first.rightTangent * dx * first.rightTangentMagnitude;
                m1 = second.leftTangent * dx * second.leftTangentMagnitude;
            } else {
                t = 0.0f;
                m0 = 0;
                m1 = 0;
            }

            return hermite(t, first.value, m0, m1, second.value);
        }

        static T hermite(float t, T p0, float m0, float m1, T p1) {
            float a = (2.0f * p0) + m0 - (2.0f * p1) + m1;
            float b = (-3.0f * p0) - (2.0f * m0) + (3.0f * p1) - m1;
            float c = m0;
            float d = p0;
            return t * (t * (a * t + b) + c) + d;
        }*/

private:
    std::vector<OPKeyframe> _keyframes;
    struct CurveCache {
        int lastIndex;
        float lastMin, lastMax;
    };
    mutable CurveCache _cache;
    void sortKeyframes() { std::sort(_keyframes.begin(), _keyframes.end()); }

public:
	void invalidateCache() {
		_cache.lastIndex = -1;
		_cache.lastMin = std::numeric_limits<float>::max();
		_cache.lastMax = std::numeric_limits<float>::min();
	}
	
    void addKeyframe(OPKeyframe&& keyframe) {
        _keyframes.push_back(std::move(keyframe));
        sortKeyframes();
    }

    void addKeyframe(const OPKeyframe& keyframe) {
        _keyframes.push_back(keyframe);
        sortKeyframes();
    }

    OPKeyframe addKeyframe(float time, float leftTangent, float leftTangentMagnitude, float rightTangent, float rightTangentMagnitude, T value) {
        OPKeyframe keyframe;
        keyframe.time = time;
        keyframe.value = value;
        keyframe.interpolation = OPKeyframe::OPInterpolation::CUBIC;
        keyframe.leftTangent = leftTangent;
        keyframe.rightTangent = rightTangent;
        keyframe.leftTangentMagnitude = leftTangentMagnitude;
        keyframe.rightTangentMagnitude = rightTangentMagnitude;
		invalidateCache();
        addKeyframe(keyframe);
        return keyframe;
    }
    const std::vector<OPKeyframe>& getKeyframes() const { return _keyframes; }
    OPKeyframe& first() { return _keyframes[0]; }

    OPKeyframe& last() { return _keyframes[_keyframes.size() - 1]; }

    const OPKeyframe& first() const { return _keyframes[0]; }

    const OPKeyframe& last() const { return _keyframes[_keyframes.size() - 1]; }

    OPKeyframe addFlatKeyframe(float time, T value, float leftMagnitude = 1, float rightMagnitude = 1) {
        return addKeyframe(time, M_PI, leftMagnitude, 0, rightMagnitude, value);
    }
    OPKeyframe addWeakKeyframe(float time, T value) {
        return addKeyframe(time, M_PI, 0, 0, 0, value);
    }
    OPKeyframe addEaseInKeyframe(float time, T value) { return addKeyframe(time, M_PI, 1, 0, 0, value); }

    OPKeyframe addEaseOutKeyframe(float time, T value) { return addKeyframe(time, M_PI, 0, 0, 1, value); }

    OPKeyframe addLinearKeyframe(float time, T value) {
        OPKeyframe keyframe{.time = time, .value = value, .interpolation = OPKeyframe::OPInterpolation::LINEAR};
        _keyframes.push_back(keyframe);
        sortKeyframes();
		invalidateCache();
        return keyframe;
    }

    std::vector<T> evaluatePoints(int nPoints, float from = 0, float to = 1) const {
#ifdef assert
        assert(nPoints > 2 && "nPoints must be larger than two.");
#endif
        std::vector<T> arr;
        arr.resize(nPoints);
        float segmentSize = (to - from) / ((float) nPoints - 1);
        for (int i = 0; i < nPoints; i++) {
            arr[i] = evaluate(i * segmentSize);
        }
        return arr;
    }

    T evaluate(float time) const {
        const OPKeyframe& firstKeyframe = first();
        if (time <= firstKeyframe.time) {
            return firstKeyframe.value;
        }
        const OPKeyframe& lastKeyframe = last();
        if (time >= lastKeyframe.time) {
            return lastKeyframe.value;
        }
        if (time >= _cache.lastMin && time <= _cache.lastMax) {
            int index = _cache.lastIndex;
            const OPKeyframe& start = _keyframes[index];
            const OPKeyframe& end = _keyframes[index + 1];
            return interpolate(time, start, end);
        }

        int startIndex = 0;
        OPKeyframe start;
        // Minus one because if we're after the last one, we'd just return the last value
        size_t finalIndex = _keyframes.size() - 1;
        for (; startIndex < finalIndex; ++startIndex) {
            const OPKeyframe& candidate = _keyframes[startIndex];
            if (candidate.time > time) {
                continue;
            }
            const OPKeyframe& next = _keyframes[startIndex + 1];
            if (next.time <= time) {
                continue;
            }
            start = candidate;
            break;
        }
        int endIndex = startIndex + 1;
        const OPKeyframe end = _keyframes[endIndex];
        _cache.lastIndex = startIndex;
        _cache.lastMax = start.time;
        _cache.lastMin = end.time;
        return interpolate(time, start, end);
    }
    T interpolate(float time, const OPKeyframe& start, const OPKeyframe& end) const {
        float normalizedTime = OPMath::inv_lerp(start.time, end.time, time);
        switch (start.interpolation) {
            case OPKeyframe::CUBIC:
                return cubic(normalizedTime, start, end);
                /*case OPKeyframe::HERMITE:
                    return hermite(normalizedTime, start, end);*/
            case OPKeyframe::LINEAR:
                return OPMath::lerp(start.value, end.value, normalizedTime);
        }
        throw std::runtime_error("Invalid interpolation mode");
    }
    bool isDefined() { return !_keyframes.empty(); }
    static cocos2d::Vec2 findBezierPosition(cocos2d::Vec2 p, float tangent, float magnitude, float dx) {
        float sin = std::sin(tangent);
        float cos = std::cos(tangent);
        return p + cocos2d::Vec2(cos, sin) * magnitude * (dx / 4);
    }
};

#endif

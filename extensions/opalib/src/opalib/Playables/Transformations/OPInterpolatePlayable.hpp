//
// Created by bruno on 31/05/2021.
//

#ifndef OPALIBTEMPLATE_OPPOSITIONPLAYABLE_HPP
#define OPALIBTEMPLATE_OPPOSITIONPLAYABLE_HPP
#include <FairyGUI.h>
#include <cocos2d.h>

#include <Math/OPCurve.hpp>
#include <Playables/OPPlayable.hpp>
#include <Math/OPMath.hpp>

#include "Playables/Transformations/OPAnchor.hpp"

struct OPPositionAnchor : public OPAnchor<cocos2d::Vec2> {
public:
    OPPositionAnchor(
        const cocos2d::Vec2& staticValue,
        cocos2d::Vec2 offset = cocos2d::Vec2::ZERO
    );
    OPPositionAnchor(
        fairygui::GObject* anchor,
        cocos2d::Vec2 offset = cocos2d::Vec2::ZERO,
        fairygui::GObject* relativeTo = nullptr
    );
    OPPositionAnchor();
protected:
    cocos2d::Vec2 offset;
    cocos2d::Vec2 extract(fairygui::GObject* object) const override;
};

class OPInterpolatePlayable : public OPPlayable {
    /**
     * TODO: Add template "Interpolator", because Bézier Curve cannot 100% linearly interpolate
     * by time, so somebody might wanna use a basic lerp instead of a Curve.
     */
public:
    struct OPInterpolationKeyframe {
    public:
        float time;
        OPPositionAnchor position;
        bool blockOffset;
    };
private:
    OPFrameTimeType offsetType, interpolationType;
    std::vector<OPInterpolationKeyframe> keyframes;
    fairygui::GObject* object;
    OPCurve<cocos2d::Vec2> offset;
    OPCurve<float> interpolation;

public:
    OPInterpolatePlayable(
        const cocos2d::Vec2& from,
        const cocos2d::Vec2& to,
        fairygui::GObject* object,
        OPFrameTimeType offsetType = OPFrameTimeType::RELATIVE,
        OPFrameTimeType interpolationType = OPFrameTimeType::RELATIVE
    );

    OPInterpolatePlayable(
        fairygui::GObject* from,
        fairygui::GObject* to,
        fairygui::GObject* object,
        OPFrameTimeType offsetType = OPFrameTimeType::RELATIVE,
        OPFrameTimeType interpolationType = OPFrameTimeType::RELATIVE
    );

    OPInterpolatePlayable(
        OPPositionAnchor from,
        OPPositionAnchor to,
        fairygui::GObject* object,
        const OPCurve<float>& interpolation,
        const OPCurve<cocos2d::Vec2>& offset,
        OPFrameTimeType offsetType = OPFrameTimeType::RELATIVE,
        OPFrameTimeType interpolationType = OPFrameTimeType::RELATIVE
    );
    OPInterpolatePlayable(
        std::vector<OPInterpolationKeyframe> keyframes,
        fairygui::GObject* object,
        const OPCurve<cocos2d::Vec2>& offset,
        const OPCurve<float>& interpolation,
        OPFrameTimeType offsetType = OPFrameTimeType::RELATIVE,
        OPFrameTimeType interpolationType = OPFrameTimeType::RELATIVE
    );
    void evaluate(OPFrameTimes times) override;
    float getInterpolationValue(float nextTime, float keyframeTime, const OPFrameTimes& times, const OPFrameTimeType& type) const;
};

#endif

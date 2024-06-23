//
// Created by bruno on 09/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPSCALEPLAYABLE_HPP
#define OPALIB_CINEMATICS_OPSCALEPLAYABLE_HPP

#include <FairyGUI.h>
#include <cocos2d.h>

#include <Playables/Transformations/OPAnchor.hpp>
#include <Playables/OPPlayable.hpp>
#include <Math/OPCurve.hpp>

struct OPScaleAnchor : public OPAnchor<cocos2d::Vec2> {
public:
    OPScaleAnchor(const cocos2d::Vec2& staticValue);

    OPScaleAnchor(fairygui::GObject* anchor);
protected:
    cocos2d::Vec2 extract(fairygui::GObject* object) const override;
protected:
};

class OPScalePlayable : public OPPlayable {
private:
    OPCurve<cocos2d::Vec2> _scaleCurve;
    fairygui::GObject* _target;
    OPFrameTimeType _timeType;
    float baseMultiplier;
public:
    OPScalePlayable(
        const OPCurve<cocos2d::Vec2>& scaleCurve,
        fairygui::GObject* target,
        OPFrameTimeType timeType = OPFrameTimeType::RELATIVE,
        float baseMultiplier = 1
    );

    void evaluate(OPFrameTimes times) override;
    float getBaseMultiplier() const;
    void setBaseMultiplier(float baseMultiplier);
};

#endif // OPALIB_CINEMATICS_OPSCALEPLAYABLE_HPP

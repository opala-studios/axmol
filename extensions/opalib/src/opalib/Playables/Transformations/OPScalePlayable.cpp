//
// Created by bruno on 09/06/2021.
//

#include "OPScalePlayable.hpp"

cocos2d::Vec2 OPScaleAnchor::extract(fairygui::GObject* object) const {
    return object->getScale();
}

OPScaleAnchor::OPScaleAnchor(fairygui::GObject* anchor) :
    OPAnchor(anchor) {}

OPScaleAnchor::OPScaleAnchor(const cocos2d::Vec2& staticValue) :
    OPAnchor(staticValue) {}

void OPScalePlayable::evaluate(OPFrameTimes times) {
    cocos2d::Vec2 scale = _scaleCurve.evaluate(times.get(_timeType));
    scale *= baseMultiplier;
    _target->setScale(scale.x, scale.y);
}

OPScalePlayable::OPScalePlayable(
    const OPCurve<cocos2d::Vec2>& scaleCurve,
    fairygui::GObject* target,
    OPFrameTimeType timeType,
    float baseMultiplier
)
    :
    _scaleCurve(scaleCurve),
    _target(target),
    _timeType(timeType),
    baseMultiplier(baseMultiplier) {}

float OPScalePlayable::getBaseMultiplier() const {
    return baseMultiplier;
}
void OPScalePlayable::setBaseMultiplier(float baseMultiplier) {
    OPScalePlayable::baseMultiplier = baseMultiplier;
}

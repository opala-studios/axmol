//
// Created by bruno on 09/06/2021.
//

#include <array>
#include "OPOpacityPlayable.hpp"

void OPOpacityPlayable::evaluate(OPFrameTimes times)
{
    _target->setAlpha(_alphaCurve.evaluate(times.get(_timeType)));
}

OPOpacityPlayable::OPOpacityPlayable(
    const OPCurve<float>& alphaCurve,
    fairygui::GImage* target,
    OPFrameTimeType timeType
)
    : _alphaCurve(alphaCurve), _target(target),
      _timeType(timeType) {}

//
// Created by bruno on 09/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPOPACITYPLAYABLE_HPP
#define OPALIB_CINEMATICS_OPOPACITYPLAYABLE_HPP

#include <Playables/Transformations/OPAnchor.hpp>
#include <Playables/OPPlayable.hpp>
#include <Math/OPCurve.hpp>

class OPOpacityPlayable : public OPPlayable {
private:
    const OPCurve<float> _alphaCurve;
    fairygui::GImage* _target;
    OPFrameTimeType _timeType;
public:
    OPOpacityPlayable(
        const OPCurve<float>& alphaCurve,
        fairygui::GImage* target,
        OPFrameTimeType timeType = OPFrameTimeType::RELATIVE
    );

    void evaluate(OPFrameTimes times) override;
};

#endif // OPALIB_CINEMATICS_OPOPACITYPLAYABLE_HPP

//
// Created by bruno on 09/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPANIMATIONPLAYABLE_HPP
#define OPALIB_CINEMATICS_OPANIMATIONPLAYABLE_HPP

#include <FairyGUI.h>

#include <Playables/OPPlayable.hpp>
class OPAnimationPlayable : public OPPlayable {
public:

    OPAnimationPlayable(
        fairygui::Transition* transition,
        float evalTimeBegin = 0,
        float evalTimeEnd = 1,
        bool snapToStart = true,
        bool snapToEnd = true
    );
    void begin(OPFrameTimes times) override;
    void evaluate(OPFrameTimes times) override;
    void end(OPFrameTimes times) override;
protected:
    bool snapToStart;
    bool snapToEnd;
    fairygui::Transition* _transition;
    void evalAnim(float progress);
    float evalTimeBegin, evalTimeEnd;
};

#endif // OPALIB_CINEMATICS_OPANIMATIONPLAYABLE_HPP

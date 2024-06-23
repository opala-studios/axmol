//
// Created by bruno on 09/06/2021.
//

#include "OPAnimationPlayable.hpp"

void OPAnimationPlayable::evaluate(OPFrameTimes times) {
    evalAnim(times.progress);
}
void OPAnimationPlayable::evalAnim(float progress) {
    auto time = progress * _transition->getTotalDuration();
    // TODO: Make an evaluate method on transition, probably on our FairyGUI fork
    _transition->play(1, 0, time, time, nullptr);
}

OPAnimationPlayable::OPAnimationPlayable(
    fairygui::Transition* transition,
    float evalTimeBegin,
    float evalTimeEnd,
    bool snapToStart,
    bool snapToEnd
) :
    _transition(transition),
    evalTimeBegin(evalTimeBegin),
    evalTimeEnd(evalTimeEnd),
    snapToStart(snapToStart),
    snapToEnd(snapToEnd) {}

    void OPAnimationPlayable::begin(OPFrameTimes times) {

    if (snapToStart) {
        evalAnim(evalTimeBegin);
    }
}

void OPAnimationPlayable::end(OPFrameTimes times) {
    if (snapToEnd) {
        evalAnim(evalTimeEnd);
    }
}

//
// Created by bruno on 16/07/2021.
//

#include "OPLoopAnimationPlayable.hpp"
OPLoopAnimationPlayable::OPLoopAnimationPlayable(float loopDuration, fairygui::Transition* transition) :
    _loopDuration(loopDuration),
    _transition(transition) {}

void OPLoopAnimationPlayable::evaluate(OPFrameTimes times) {
    float progress = fmod(times.time, _loopDuration);
    auto time = progress * _transition->getTotalDuration();
    // TODO: Make an evaluate method on transition, probably on our FairyGUI fork
    _transition->play(1, 0, time, time, nullptr);
}

//
// Created by bruno on 16/07/2021.
//

#ifndef OPALIB_CINEMATICS_OPLOOPANIMATIONPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPLOOPANIMATIONPLAYABLE_HPP_
#include <Playables/OPPlayable.hpp>
#include <FairyGUI.h>
class OPLoopAnimationPlayable : public OPPlayable {
private:
    float _loopDuration;
    fairygui::Transition * _transition;
public:
    OPLoopAnimationPlayable(float loopDuration, fairygui::Transition* transition);
    void evaluate(OPFrameTimes times) override;

};

#endif //OPALIB_CINEMATICS_OPLOOPANIMATIONPLAYABLE_HPP_

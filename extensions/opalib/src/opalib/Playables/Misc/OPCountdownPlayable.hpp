//
// Created by bruno on 28/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPCOUNTDOWNPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPCOUNTDOWNPLAYABLE_HPP_

#include <FairyGUI.h>

#include <Playables/OPPlayable.hpp>
#include <Math/OPMath.hpp>
class OPCountdownPlayable : public OPPlayable {
private:
    fairygui::GTextField* _textField;
    float _from;
    float _to;
    float _extra;
public:
    OPCountdownPlayable(
        fairygui::GTextField* textField,
        float entryModifier,
        float exitModifier,
        float extra = 0
    );
    void evaluate(OPFrameTimes times) override;
};

#endif //OPALIB_CINEMATICS_OPCOUNTDOWNPLAYABLE_HPP_

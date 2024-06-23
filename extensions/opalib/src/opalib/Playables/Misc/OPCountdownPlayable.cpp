//
// Created by bruno on 28/06/2021.
//

#include "OPCountdownPlayable.hpp"
#include <cmath>
OPCountdownPlayable::OPCountdownPlayable(
    fairygui::GTextField* textField,
    float entryModifier,
    float exitModifier,
    float extra
) :
    _textField(textField),
    _from(entryModifier),
    _to(exitModifier),
    _extra(extra) {

}
void OPCountdownPlayable::evaluate(OPFrameTimes times) {
    float value = OPMath::lerp(_from, _to, times.progress);
    int intValue = static_cast<int>(value + _extra);
    _textField->setText(std::to_string(intValue));
}

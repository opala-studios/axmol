//
// Created by bruno on 09/06/2021.
//

#include "OPVisibilityPlayable.hpp"

void OPVisibilityPlayable::begin(OPFrameTimes times)
{
    _target->setVisible(_active);
}

void OPVisibilityPlayable::end(OPFrameTimes times)
{
    _target->setVisible(!_active);
}

OPVisibilityPlayable::OPVisibilityPlayable(bool active, fairygui::GObject *target) : _active(
        active), _target(target) {}

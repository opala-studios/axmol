//
// Created by bruno on 16/06/2021.
//

#include "OPPositionPlayable.hpp"
void OPPositionPlayable::evaluate(OPFrameTimes times) {
    cocos2d::Vec2 found = _position.evaluate(times.progress);
    _target->setPosition(found.x, found.y);
}

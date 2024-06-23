//
// Created by bruno on 14/07/2021.
//

#include "OPControllerIndexPlayable.hpp"
void OPControllerIndexPlayable::begin(OPFrameTimes times) {
    _controller->setSelectedIndex(_entryIndex);
}
void OPControllerIndexPlayable::end(OPFrameTimes times) {
    _controller->setSelectedIndex(_exitIndex);
}
OPControllerIndexPlayable::OPControllerIndexPlayable(fairygui::GController* controller, size_t entryIndex, size_t exitIndex) :
    _controller(controller),
    _entryIndex(entryIndex),
    _exitIndex(exitIndex) {}

//
// Created by bruno on 14/07/2021.
//

#ifndef OPALIB_CINEMATICS_OPCONTROLLERINDEXPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPCONTROLLERINDEXPLAYABLE_HPP_
#include <FairyGUI.h>
#include <Playables/OPPlayable.hpp>

class OPControllerIndexPlayable : public OPPlayable {
private:
    fairygui::GController* _controller;
    size_t _entryIndex, _exitIndex;
public:
    OPControllerIndexPlayable(fairygui::GController* controller, size_t entryIndex, size_t exitIndex);
    void begin(OPFrameTimes times) override;
    void end(OPFrameTimes times) override;
};

#endif //OPALIB_CINEMATICS_OPCONTROLLERINDEXPLAYABLE_HPP_

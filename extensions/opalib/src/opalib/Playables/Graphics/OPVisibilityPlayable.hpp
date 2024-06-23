//
// Created by bruno on 09/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPVISIBILITYPLAYABLE_HPP
#define OPALIB_CINEMATICS_OPVISIBILITYPLAYABLE_HPP

#include <Playables/Transformations/OPAnchor.hpp>
#include <Playables/OPPlayable.hpp>

class OPVisibilityPlayable : public OPPlayable
{
private:
    bool _active;
    fairygui::GObject* _target;

public:
    OPVisibilityPlayable(bool active, fairygui::GObject *target);

    void begin(OPFrameTimes times) override;

    void end(OPFrameTimes times) override;
};

#endif // OPALIB_CINEMATICS_OPVISIBILITYPLAYABLE_HPP

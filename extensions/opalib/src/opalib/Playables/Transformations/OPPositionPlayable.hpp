#ifndef OPALIB_CINEMATICS_OPPOSITIONPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPPOSITIONPLAYABLE_HPP_

#include <Math/OPCurve.hpp>
#include <Playables/OPPlayable.hpp>
#include <cocos2d.h>
#include <FairyGUI.h>

class OPPositionPlayable : public OPPlayable {
private:
    OPCurve<cocos2d::Vec2> _position;
    fairygui::GObject* _target;
public:
    void evaluate(OPFrameTimes times) override;
};

#endif
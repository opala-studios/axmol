//
// Created by bruno on 09/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPANCHOR_HPP
#define OPALIB_CINEMATICS_OPANCHOR_HPP

#include <FairyGUI.h>
#include <cocos2d.h>

#include <Playables/OPPlayable.hpp>
#include <Math/OPMath.hpp>
template<typename T>
class OPAnchor {
public:
    enum OPAnchorMode {
        STATIC,
        ANCHORED,
        INITIAL_VALUE,
        INVALID
    };

protected:
    union {
        T staticValue;
        struct {
            fairygui::GObject* anchor;
            fairygui::GObject* relativeTo;
        };
        struct {
            fairygui::GObject* initialAnchor;
            T initialValue;
        };
    };

    OPAnchorMode mode;

protected:
    virtual T extract(fairygui::GObject* object) const = 0;

public:
    OPAnchor(T staticValue) :
        staticValue(staticValue),
        mode(OPAnchorMode::STATIC) {};
    OPAnchor(
        fairygui::GObject* anchor,
        fairygui::GObject* relativeTo = nullptr
    ) :
        anchor(anchor),
        relativeTo(relativeTo),
        mode(OPAnchorMode::ANCHORED) {};
    OPAnchor() :
        mode(OPAnchorMode::INVALID) {}

    T getStaticValue() const {
        return staticValue;
    }

    fairygui::GObject* getAnchor() const {
        return anchor;
    }

    OPAnchorMode getMode() const {
        return mode;
    }

    T get() const {
        switch (mode) {
            case OPAnchorMode::STATIC:
                return staticValue;
            case OPAnchorMode::ANCHORED:
                return extract(anchor);
            default:
                throw std::runtime_error("Invalid mode");
        }
    }
};

#endif // OPALIB_CINEMATICS_OPANCHOR_HPP

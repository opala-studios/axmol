//
// Created by Ricardo on 11/25/2020.
//

#include "Vibrator.hpp"


namespace opalib {

    Vibrator::Vibrator() {

    }

    Vibrator::~Vibrator() {

    }

    void Vibrator::vibrate(int milliseconds){
        if (_enabled){
            doVibration(milliseconds);
        }
    }

    void Vibrator::setEnabled(bool enabled) {
        _enabled = enabled;
    }
}
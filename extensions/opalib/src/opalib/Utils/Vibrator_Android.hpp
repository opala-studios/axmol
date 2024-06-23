//
// Created by Ricardo on 11/25/2020.
//

#ifndef OP_VIBRATOR_ANDROID_HPP
#define OP_VIBRATOR_ANDROID_HPP

#include <Utils/Vibrator.hpp>

namespace opalib {

    class Vibrator_Android : public Vibrator {
    protected:
        void doVibration(int milliseconds) override;
    };

}

#endif //OP_VIBRATOR_ANDROID_HPP

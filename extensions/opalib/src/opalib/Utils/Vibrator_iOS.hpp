//
//  Vibrator_iOS.hpp
//  opalib-ios
//
//  Created by Ricardo on 26/11/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef Vibrator_iOS_h
#define Vibrator_iOS_h

#include <Utils/Vibrator.hpp>

namespace opalib {
    class Vibrator_iOS : public Vibrator {
    public:
        Vibrator_iOS();
    protected:
        void doVibration(int milliseconds) override;
        
    private:
        void* _impl {nullptr};
    };

}


#endif /* Vibrator_iOS_h */

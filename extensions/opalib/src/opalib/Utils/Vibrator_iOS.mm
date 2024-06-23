//
//  Vibrator_iOS.mm
//  opalib-ios
//
//  Created by Ricardo on 26/11/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include <Utils/Vibrator_iOS.hpp>
#include <Utils/Vibrator_iOSImpl.h>

namespace opalib {
    
    Vibrator* Vibrator::createForPlatform(){
        return new Vibrator_iOS();
    }
    
    Vibrator_iOS::Vibrator_iOS(){
        _impl = [[Vibrator_iOSImpl alloc] init];
    }
    
    void Vibrator_iOS::doVibration(int milliseconds){
        [((Vibrator_iOSImpl*)_impl) vibratePeek];
    }
}


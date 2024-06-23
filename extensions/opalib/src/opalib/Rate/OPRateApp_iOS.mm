//
//  OPRateApp_iOS.cpp
//  opalib-ios
//
//  Created by Talles Borges  on 19/08/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include <stdio.h>
#include "OPRateApp.hpp"
#import <StoreKit/StoreKit.h>

void OPRateApp::rate() {
    if (@available(iOS 10.3, *)) {
        [SKStoreReviewController requestReview];
    }
}

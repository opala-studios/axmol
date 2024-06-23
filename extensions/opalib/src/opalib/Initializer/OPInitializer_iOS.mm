//
//  OPInitializer_iOS.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#include "OPInitializer_iOS.hpp"

#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <FirebaseCore/FirebaseCore.h>

void OPInitializer_iOS::initialize()
{
    [FIRApp configure];
    OPInitializer::initialize();
}

bool OPInitializer_iOS::needsIDFAConfirmation(){
    if (@available(iOS 14.5, *)) {
        ATTrackingManagerAuthorizationStatus status = [ATTrackingManager trackingAuthorizationStatus];
        return  status != ATTrackingManagerAuthorizationStatusAuthorized &&
        status != ATTrackingManagerAuthorizationStatusDenied;
    } else {
        return false;
    }
}

void OPInitializer_iOS::askForIDFAConfirmation(const std::function<void(bool)>& onCompletion){
    if(@available(iOS 14.5, *)){
        auto completion = onCompletion;
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(status == ATTrackingManagerAuthorizationStatusAuthorized);
            });
        }];
    }
    else {
        onCompletion(true);
    }
}


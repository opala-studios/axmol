//
//  OPNotifications_iOS.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#include "OPNotifications_iOS.hpp"
#include "OPNotifications_iOSImpl.h"

OPNotifications_iOS::OPNotifications_iOS()
{
    impl = [[OPNotifications_iOSImpl alloc] init];
    
    [impl initialize];
}

bool OPNotifications_iOS::isRegisteredForNotifications()
{
    return UIApplication.sharedApplication.isRegisteredForRemoteNotifications;
}

void OPNotifications_iOS::askForPermission()
{
    OPNotificationManager::askForPermission();
    
    [((OPNotifications_iOSImpl*)impl) registerForRemoteNotifications];
}

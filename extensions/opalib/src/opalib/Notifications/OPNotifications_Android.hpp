//
//  OPNotifications_Android.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#ifndef OPNotifications_Android_hpp
#define OPNotifications_Android_hpp

#include "OPNotificationManager.hpp"

class OPNotifications_Android : public OPNotificationManager
{
public:
    
    OPNotifications_Android();
    
    virtual void askForPermission() override;
    virtual bool isRegisteredForNotifications() override;
};

#endif /* OPNotifications_Android_hpp */

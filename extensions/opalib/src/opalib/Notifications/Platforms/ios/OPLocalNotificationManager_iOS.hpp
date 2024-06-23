//
//  OPLocalNotificationManager_iOS.hpp
//  opalib-ios
//
//  Created by Opala Studios on 09/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPLocalNotificationManager_iOS_hpp
#define OPLocalNotificationManager_iOS_hpp

#include <opalib/Notifications/OPLocalNotificationManager.hpp>

class OPLocalNotificationManager_iOS : public OPLocalNotificationManager
{
public:
    OPLocalNotificationManager_iOS();
    
    virtual ~OPLocalNotificationManager_iOS();

    void setup() override;
    
    void unscheduleNotification(const std::string& actionName) override;
    
protected:
    
    void setupScheduleNotification(const std::string& actionName,
                                   const std::string& title,
                                   const std::string& message,
                                   const int hours,
                                   const int minutes,
                                   const int timeRepeat,
                                   const bool repeat) override;
    
private:
    void* _imp;
};

#endif /* OPLocalNotificationManager_iOS_hpp */

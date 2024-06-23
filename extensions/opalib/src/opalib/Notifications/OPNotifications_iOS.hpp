//
//  OPNotifications_iOS.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#ifndef OPNotifications_iOS_hpp
#define OPNotifications_iOS_hpp

#include "OPNotificationManager.hpp"

class OPNotifications_iOS : public OPNotificationManager
{
private:
    
    void* impl {0};
    
public:
    
    OPNotifications_iOS();
    
    virtual void askForPermission() override;
    virtual bool isRegisteredForNotifications() override;
    
    void* getImpl() { return impl; }
};

#endif /* OPNotifications_iOS_hpp */

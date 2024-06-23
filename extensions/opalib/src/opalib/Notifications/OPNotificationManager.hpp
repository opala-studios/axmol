//
//  OPNotifications.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#ifndef OPNotifications_hpp
#define OPNotifications_hpp

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <cocos2d/external/json/document.h>

class OPNotificationManager {
public:

    struct Notification {
        rapidjson::Document payload;
        bool cameFromBackground;
    };

    typedef std::function<bool(Notification&)> NotificationListenerCallback;

protected:

    struct NotificationListener {
        NotificationListenerCallback callback;
        size_t id;
    };

public:
    
    void initialize();
    
    static OPNotificationManager* shared();
    
    bool alreadyAskedForPermission();
    virtual void askForPermissionIfNeeded();
    virtual bool isRegisteredForNotifications() = 0;

    void enqueueNotification(Notification&& notification);
    void dispatchNotification();

    size_t addNotificationListener(const NotificationListenerCallback& callback);
    void removeNotificationListener(size_t listenerId);
protected:

    virtual void askForPermission();
    virtual void setup();

private:

    void setAlreadyAskedForPermission(bool asked);
private:
    std::vector<Notification> _notificationQueue;
    std::vector<NotificationListener> _notificationListeners;
    size_t _listenerIdCounter = 0;
    std::mutex _notificationQueueMutex;

};

#endif /* OPNotifications_hpp */

//
//  OPNotifications.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#include "OPNotificationManager.hpp"
#include "../UserData/OPUserData.hpp"
#include "../Debug/OPDebug.hpp"

#include <cocos2d.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OPNotifications_iOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OPNotifications_Android.hpp"
#endif

OPNotificationManager* OPNotificationManager::shared()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static OPNotificationManager* instance = new OPNotifications_iOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static OPNotificationManager* instance = new OPNotifications_Android();
#endif
    
    return instance;
}

void OPNotificationManager::initialize()
{
    setup();
}

void OPNotificationManager::setup()
{
}

void OPNotificationManager::askForPermission()
{
    setAlreadyAskedForPermission(true);
}

void OPNotificationManager::askForPermissionIfNeeded()
{
    if (!alreadyAskedForPermission()) {
        askForPermission();
    }
}

bool OPNotificationManager::alreadyAskedForPermission()
{
    return OPUserData::shared()->getBool("notifications_asked_permission", false);
}

void OPNotificationManager::setAlreadyAskedForPermission(bool asked)
{
    OPUserData::shared()->setBool("notifications_asked_permission", asked);
}

void OPNotificationManager::enqueueNotification(Notification&& notification) {
    std::lock_guard<std::mutex> lock(_notificationQueueMutex);
    _notificationQueue.emplace_back(std::move(notification));

    //MAYBE we should take care of this in another way
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
       dispatchNotification();
    });
}

void OPNotificationManager::dispatchNotification() {
    std::lock_guard<std::mutex> lock(_notificationQueueMutex);
    if (_notificationQueue.empty()){
        return;
    }
    
    std::vector<OPNotificationManager::Notification> pendingNotifications;
    
    for (auto& notification : _notificationQueue){
        bool consumed = false;
        for(auto& listener : _notificationListeners){

            //It's the listener responsibility to consume the notification
            //If it fails to consume, the notification will continue to exist in our queue
            if (listener.callback(notification)){
                consumed = true;
                break;
            }
        }
        if (!consumed){
            pendingNotifications.emplace_back(std::move(notification));
        }
    }
    std::swap(_notificationQueue, pendingNotifications);
}

size_t OPNotificationManager::addNotificationListener(const NotificationListenerCallback &listener) {
    size_t id = _listenerIdCounter++;
    _notificationListeners.emplace_back(NotificationListener{listener, id});
    return id;
}

void OPNotificationManager::removeNotificationListener(size_t listenerId) {
    _notificationListeners.erase(
        std::remove_if(
            _notificationListeners.begin(),
            _notificationListeners.end(),
            [listenerId](const NotificationListener& listener){
                return listener.id == listenerId;
            }),
        _notificationListeners.end()
        );
}

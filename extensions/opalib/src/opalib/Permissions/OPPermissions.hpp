#ifndef OPPermissions_hpp
#define OPPermissions_hpp

#include <cocos2d.h>

class OPPermissions
{
public:
    
    typedef std::function<void(bool)> SEL_PermissionRequestHandler;

    // MARK: - Permissions
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static SEL_PermissionRequestHandler readExternalStorageHandler;
    static SEL_PermissionRequestHandler writeExternalStorageHandler;
    
    static void requestReadExternalStorage(const SEL_PermissionRequestHandler& handler = 0);
    static void requestWriteExternalStorage(const SEL_PermissionRequestHandler& handler = 0);
#endif

    static SEL_PermissionRequestHandler microphoneHandler;
    static void requestMicrophone(const SEL_PermissionRequestHandler& handler = 0);
    static bool isMicrophoneAuthorized();
    
    static void openAppSettings();
    
    // MARK: - Helpers

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static void handlePermissionResult(SEL_PermissionRequestHandler& handler, bool granted);
    static void requestAndroidPermission(const std::string& methodName);
#endif
};

#endif

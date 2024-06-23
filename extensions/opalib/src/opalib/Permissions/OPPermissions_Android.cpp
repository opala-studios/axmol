#include "OPPermissions.hpp"
#include <jni/JniHelper.h>

USING_NS_CC;

void OPPermissions::requestReadExternalStorage(const SEL_PermissionRequestHandler & handler)
{
    readExternalStorageHandler = handler;
    requestAndroidPermission("requestReadExternalStorage");
}

void OPPermissions::requestWriteExternalStorage(const SEL_PermissionRequestHandler & handler)
{
    writeExternalStorageHandler = handler;
    requestAndroidPermission("requestWriteExternalStorage");
}

void OPPermissions::requestMicrophone(const SEL_PermissionRequestHandler & handler)
{
    microphoneHandler = handler;
    requestAndroidPermission("requestMicrophone");
}

void OPPermissions::requestAndroidPermission(const std::string& methodName)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/permissions/Permissions", methodName);
}

void OPPermissions::handlePermissionResult(SEL_PermissionRequestHandler& handler, bool granted)
{
    if (handler) {
        handler(granted);
    }

    handler = nullptr;
}

void OPPermissions::openAppSettings()
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/app/Application", "openAppSettings");
}

bool OPPermissions::isMicrophoneAuthorized()
{
    return JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/permissions/Permissions", "isMicrophoneAuthorized");
}

extern "C" {
void Java_com_opalastudios_opalib_permissions_Permissions_onRequestReadExternalStorageResult(JNIEnv* env, jobject obj, jboolean granted) {
    OPPermissions::handlePermissionResult(OPPermissions::readExternalStorageHandler, granted);
}
void Java_com_opalastudios_opalib_permissions_Permissions_onRequestWriteExternalStorageResult(JNIEnv* env, jobject obj, jboolean granted) {
    OPPermissions::handlePermissionResult(OPPermissions::writeExternalStorageHandler, granted);
}
void Java_com_opalastudios_opalib_permissions_Permissions_onRequestMicrophoneResult(JNIEnv* env, jobject obj, jboolean granted) {
    OPPermissions::handlePermissionResult(OPPermissions::microphoneHandler, granted);
}
}

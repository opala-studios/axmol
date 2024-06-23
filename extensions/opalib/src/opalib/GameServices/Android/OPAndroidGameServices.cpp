//
// Created by Ricardo on 01/02/22.
//

#include <jni.h>
#include <opalib/Misc/OPCocosThread.hpp>
#include "OPAndroidGameServices.hpp"

static OPAndroidGameServices* s_instance = nullptr;
static const char* const GAME_SERVICES_CLASS_NAME = "com/opalastudios/opalib/gameservices/GameServices";

extern "C" {

JNIEXPORT void JNICALL Java_com_opalastudios_opalib_gameservices_GameServices_cppOnSignInResult (JNIEnv *env, jobject thiz, jint resultCode, jstring id, jstring userName){

    OPGameServices::Account* account = nullptr;
    if (resultCode == 0){
        account = new OPGameServices::Account(
                cocos2d::JniHelper::jstring2string(id),
                cocos2d::JniHelper::jstring2string(userName)
                );
    }
    s_instance->onSignInResult(static_cast<OPGameServices::ResultCode>(resultCode), account);
}

JNIEXPORT void JNICALL Java_com_opalastudios_opalib_gameservices_GameServices_cppOnSnapshotMetadataLoadedComplete(JNIEnv *env, jobject thiz, jint resultCode){
    s_instance->onSnapshotMetadataLoadedCompleted(static_cast<OPGameServices::ResultCode>(resultCode));
}

JNIEXPORT void JNICALL Java_com_opalastudios_opalib_gameservices_GameServices_cppOnSnapshotMetadataLoaded(JNIEnv *env, jobject thiz, jstring snapshotName, jlong timestamp){
    s_instance->onSnapshotMetadataLoaded(cocos2d::JniHelper::jstring2string(snapshotName), timestamp);
}

JNIEXPORT void JNICALL Java_com_opalastudios_opalib_gameservices_GameServices_cppOnSnapshotContentLoaded(JNIEnv *env, jobject thiz, jint resultCode, jbyteArray bytes){

    jbyte* buffer = nullptr;
    jsize length = 0;
    if (bytes){
        length = env->GetArrayLength(bytes);
        buffer = new jbyte[length];
        env->GetByteArrayRegion(bytes, 0, length, buffer);
    }
    s_instance->onSnapshotContentLoaded(static_cast<OPGameServices::ResultCode>(resultCode), buffer, length);
}

JNIEXPORT void JNICALL Java_com_opalastudios_opalib_gameservices_GameServices_cppOnSnapshotContentSaved(JNIEnv *env, jobject thiz, jint resultCode){
    s_instance->onSnapshotContentSaved(static_cast<OPGameServices::ResultCode>(resultCode));
}

}

OPGameServices* OPGameServices::shared(){
    static OPAndroidGameServices instance;
    return &instance;
}

OPAndroidGameServices::OPAndroidGameServices() {
    s_instance = this;
}

void OPAndroidGameServices::initialize() {
    cocos2d::JniHelper::callStaticVoidMethod(GAME_SERVICES_CLASS_NAME, "initialize");
    _initialized = true;
}

void OPAndroidGameServices::signIn(const OPGameServices::SignInCallback &callback) {
    if (!_initialized){
        callback(OPGameServices::ResultCode::FAILED_NOT_INITIALIZED, nullptr);
        return;
    }
    if (_signingIn){
        callback(OPGameServices::ResultCode::FAILED_ALREADY_SIGNING_IN, nullptr);
        return;
    }
    if (_signedAccount){
        callback(ResultCode::FAILED_ALREADY_SIGNED_IN, _signedAccount.get());
        return;
    }

    _signingIn = true;
    _signInCallback = callback;
    cocos2d::JniHelper::callStaticVoidMethod(GAME_SERVICES_CLASS_NAME, "signIn");
}

void OPAndroidGameServices::loadSnapshotMetadata(const OPGameServices::LoadSnapshotMetadataCallback &callback) {
    if (!_initialized){
        callback(OPGameServices::ResultCode::FAILED_NOT_INITIALIZED);
        return;
    }
    if (!_signedAccount){
        callback(ResultCode::FAILED_NOT_SIGNED_IN);
        return;
    }
    _loadSnapshotMetadataCallback = callback;
    cocos2d::JniHelper::callStaticVoidMethod(GAME_SERVICES_CLASS_NAME, "loadSnapshotMetadata");
}

void OPAndroidGameServices::saveSnapshotContent(const std::string& snapshotName, void* data, size_t size, const SaveSnapshotContentCallback &callback) {
    _saveSnapshotContentCallback = callback;
    auto env = cocos2d::JniHelper::getEnv();
    jbyteArray arr = env->NewByteArray(size);
    env->SetByteArrayRegion(arr, 0, size, (jbyte*)data);

    cocos2d::JniHelper::callStaticVoidMethod(GAME_SERVICES_CLASS_NAME, "saveSnapshotContent", snapshotName, arr);

    env->DeleteLocalRef(arr);
}

void OPAndroidGameServices::loadSnapshotContent(const std::string &snapshotId,
                                                const LoadSnapshotContentCallback &callback) {
    _loadSnapshotContentCallback = callback;
    cocos2d::JniHelper::callStaticVoidMethod(GAME_SERVICES_CLASS_NAME, "loadSnapshotContent", snapshotId);
}

void OPAndroidGameServices::onSignInResult(ResultCode code, OPGameServices::Account* account) {

    auto callback = _signInCallback;
    _signInCallback = nullptr;
    _signedAccount = std::unique_ptr<Account>(account);
    _signingIn = false;
    if (callback){
        callback(code, account);
    }
}

void OPAndroidGameServices::onSnapshotContentLoaded(ResultCode code, void *bytes, size_t byteCount) {
    _loadSnapshotContentCallback(code, bytes, byteCount);
}

void OPAndroidGameServices::onSnapshotMetadataLoadedCompleted(ResultCode code) {
    _loadSnapshotMetadataCallback(code);
}

void OPAndroidGameServices::onSnapshotMetadataLoaded(const std::string& snapshotName, long timestamp) {
    _signedAccount->addSnapshot(snapshotName, timestamp);
}

void OPAndroidGameServices::onSnapshotContentSaved(ResultCode resultCode) {
    _saveSnapshotContentCallback(resultCode);
}


//
// Created by Ricardo on 11/25/2020.
//

#include "Vibrator_Android.hpp"
#include <jni/JniHelper.h>
#include <cocos2d/cocos/base/CCAsyncTaskPool.h>

USING_NS_CC;

namespace opalib {

    Vibrator* Vibrator::createForPlatform(){
        return new Vibrator_Android();
    }

    void Vibrator_Android::doVibration(int milliseconds) {
        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_OTHER, [milliseconds](){
            cocos2d::JniHelper::callStaticVoidMethod("com/opalastudios/opalib/activity/OpalibActivity", "vibrate", milliseconds);
        });
    }
}



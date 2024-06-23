#include <jni/JniHelper.h>

#include "../Debug/OPDebug.hpp"
#include "../Helpers/OPString.hpp"
#include "../Misc/OPFile.hpp"
#include "../Misc/OPPath.hpp"
#include "OPDeviceSounds.hpp"

USING_NS_CC;

bool OPDeviceSounds::isAvailable()
{
    return true;
}

void OPDeviceSounds::requestList(const SEL_RequestListSoundHandler& completion)
{
    auto result = new Vector<OPDeviceSound*>();

    // lambda that will execute on ui thread
    auto mainThread = [completion, result](void* /*param*/) {
        if (completion) {
            completion(Vector<OPDeviceSound*>(*result));
        }
        delete result;
    };

    // lambda that will execute in background
    auto backgroundThread = [result]() {
        auto pathsString = JniHelper::callStaticStringMethod("com/opalastudios/opalib/files/FileUtils", "getDeviceSoundsPaths");
        auto paths = OPString::split(pathsString, ';');

        for (const auto& path : paths) {
            if (OPFile::exists(path)) {
                auto sound = new OPDeviceSound();
                sound->setName(OPPath::fileNameWithoutExtension(path));
                sound->setFilePath(path);
                result->pushBack(sound);
                sound->release();
            }
        }
    };

    cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_IO,
                                                   std::move(mainThread),
                                                   nullptr,
                                                   std::move(backgroundThread));
}

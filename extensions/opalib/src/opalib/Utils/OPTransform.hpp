//
// Created by bruno on 13/07/2021.
//

#ifndef PROJ_ANDROID_OPALIB_OPALIB_UTILS_OPTRANSFORM_HPP_
#define PROJ_ANDROID_OPALIB_OPALIB_UTILS_OPTRANSFORM_HPP_
#include <cocos2d.h>
#include <FairyGUI.h>
#include <opalib/Debug/OPDebug.hpp>
class OPTransform {
public:
    static cocos2d::Vec2 getPositionRelativeTo(
        fairygui::GObject* pObject,
        fairygui::GObject* relativeTo
    );
};

#endif //PROJ_ANDROID_OPALIB_OPALIB_UTILS_OPTRANSFORM_HPP_

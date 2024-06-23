//
// Created by bruno on 13/07/2021.
//

#include "OPTransform.hpp"
cocos2d::Vec2 OPTransform::getPositionRelativeTo(fairygui::GObject* pObject, fairygui::GObject* relativeTo) {
    auto absolute = pObject->localToGlobal(cocos2d::Vec2::ZERO);
    return relativeTo->globalToLocal(absolute);
}

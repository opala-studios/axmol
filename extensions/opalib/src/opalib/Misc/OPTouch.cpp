//
//  TouchHelper.cpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 06/06/19.
//

#include "OPTouch.hpp"

USING_NS_CC;

bool OPTouch::checkTouch(Touch *touch, Node *node)
{
    if (!touch) { return false; }
    return checkTouch(touch->getLocation(), node);
}

bool OPTouch::checkTouch(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!event) { return false; }
    return checkTouch(touch, event->getCurrentTarget());
}

bool OPTouch::checkTouch(cocos2d::Vec2 touchPosition, cocos2d::Node *node)
{
    if (!node) { return false; }
    
    auto point = node->convertToNodeSpace(touchPosition);
    auto size = node->getContentSize();
    auto rect = Rect(0, 0, size.width, size.height);
    
    return rect.containsPoint(point);
}

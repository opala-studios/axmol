//
//  OPFairyDebug.cpp
//  opalib-ios
//
//  Created by Murilo Kinape Rivabem on 08/04/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "OPFairyDebug.hpp"

fairygui::GGraph* OPFairyDebug::debugNode(
          fairygui::GComponent *node, float xExtend, float yExtend,
          cocos2d::Color4F fillColor, cocos2d::Color4F strokeColor)
{
    auto debugNode = fairygui::GGraph::create();
    debugNode->setTouchable(false);
    debugNode->setPivot(0.5, 0.5);
    debugNode->setPixelSnapping(true);
    
    auto pos = cocos2d::Vec2(node->getX() + (node->getWidth()/2),
                             node->getY() + (node->getHeight()/2));
    
    debugNode->setPosition(pos.x, pos.y);
    debugNode->drawRect(node->getWidth() + xExtend, node->getHeight() + yExtend, 1, strokeColor, fillColor);
    node->addChild(debugNode);
    
    return debugNode;
}

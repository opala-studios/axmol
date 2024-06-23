//
//  OPFairyDebug.hpp
//  opalib-ios
//
//  Created by Murilo Kinape Rivabem on 08/04/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPFairyDebug_hpp
#define OPFairyDebug_hpp

#include <FairyGUI.h>
#include <cocos2d.h>
#include "../Debug/OPDebug.hpp"

class OPFairyDebug: public OPDebug
{
public:
    static fairygui::GGraph* debugNode(
           fairygui::GComponent* node, float xExtend = 0, float yExtend = 0,
           cocos2d::Color4F fillColor = cocos2d::Color4F(1, 0, 0, 0.4),
           cocos2d::Color4F strokeColor = cocos2d::Color4F(1, 0, 0, 0));
};

#endif /* OPFairyDebug_hpp */

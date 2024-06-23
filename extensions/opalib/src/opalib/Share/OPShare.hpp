//
//  OPShare.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 07/04/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPShare_hpp
#define OPShare_hpp

#include <string>
#include <cocos2d.h>

class OPShare
{
public:
    static void shareFile(const char* filePath, cocos2d::Rect origen = cocos2d::Rect::ZERO);
    static void shareMessage(const char* message, cocos2d::Rect origen = cocos2d::Rect::ZERO);
};

#endif /* OPShare_hpp */





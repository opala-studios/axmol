//
//  TouchHelper.hpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 06/06/19.
//

#ifndef TouchHelper_hpp
#define TouchHelper_hpp

#include <cocos2d.h>

class OPTouch
{
public:
    
    static bool checkTouch(cocos2d::Vec2 touchPosition, cocos2d::Node *node);
    static bool checkTouch(cocos2d::Touch *touch, cocos2d::Node *node);
    static bool checkTouch(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif /* TouchHelper_hpp */

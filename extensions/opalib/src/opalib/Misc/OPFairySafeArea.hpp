//
//  OPFairySafeArea.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 19/03/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPFairySafeArea_hpp
#define OPFairySafeArea_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class OPFairySafeArea
{
public:

    static float left();
    static float top();
    static float width();
    static float height();

    static cocos2d::Rect rect();
};

#endif /* OPFairySafeArea_hpp */

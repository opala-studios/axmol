//
//  OPFairySafeArea.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 19/03/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPFairySafeArea.hpp"

USING_NS_CC;

float OPFairySafeArea::width()
{
    return Director::getInstance()->getSafeAreaRect().size.width;
}

float OPFairySafeArea::height()
{
    return Director::getInstance()->getSafeAreaRect().size.height;
}

float OPFairySafeArea::top()
{
    auto visibleY = Director::getInstance()->getVisibleOrigin().y;
    auto safeY = Director::getInstance()->getSafeAreaRect().origin.y;

    return safeY - visibleY;
}

float OPFairySafeArea::left()
{
    return Director::getInstance()->getSafeAreaRect().origin.x;
}

Rect OPFairySafeArea::rect()
{
    return Director::getInstance()->getSafeAreaRect();
}

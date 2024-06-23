//
//  OPWindow.cpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 06/06/19.
//

#include "OPWindow.hpp"

USING_NS_CC;

Size OPWindow::getWindowSizeInPoints()
{
    return cocos2d::Size::ZERO;
}

Size OPWindow::getWindowSizeInPixels()
{
    return Director::getInstance()->getWinSizeInPixels();
}

Rect OPWindow::getSafeArea(bool homeIndicator, bool notch)
{
    auto director = Director::getInstance();
    auto origin = director->getVisibleOrigin();
    auto size = director->getVisibleSize();

    return cocos2d::Rect(origin, size);
}

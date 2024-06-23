//
//  OPWindow.cpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 06/06/19.
//

#include "OPWindow.hpp"
#include <cocos2d.h>

#include <UIKit/UIKit.h>

cocos2d::Size OPWindow::getWindowSizeInPoints()
{
    auto size = [UIScreen mainScreen].bounds.size;
    return cocos2d::Size(size.width, size.height);
}

cocos2d::Size OPWindow::getWindowSizeInPixels()
{
    auto size = [UIScreen mainScreen].nativeBounds.size;
    return cocos2d::Size(size.width, size.height);
}

cocos2d::Rect OPWindow::getSafeArea(bool homeIndicator, bool notch)
{
    auto winSize = cocos2d::Director::getInstance()->getWinSize();
    auto deviceHeight = getWindowSizeInPoints().height;
    auto scale = winSize.height / deviceHeight;
    
    auto viewController = [[UIApplication sharedApplication] keyWindow].rootViewController;
    auto orientation = [UIApplication sharedApplication].statusBarOrientation;
    
    UIEdgeInsets safeArea = UIEdgeInsetsZero;
    
    if (@available(iOS 11, *)) {
        safeArea = viewController.view.safeAreaInsets;
    }
    
    if (!homeIndicator) {
        safeArea.bottom = 0;
    }
    
    if (!notch && orientation == UIInterfaceOrientationPortrait) {
        safeArea.top = 0;
    }
    
    auto x = safeArea.left * scale;
    auto y = safeArea.bottom * scale;
    auto width = winSize.width - safeArea.right * scale - x;
    auto height = winSize.height - safeArea.top * scale - y;
    
    return cocos2d::Rect(x, y, width, height);
}

//
//  OPWindow.h
//  DrumsCocos
//
//  Created by Adilson Tavares on 06/06/19.
//

#ifndef OPWindow_h
#define OPWindow_h

#include <cocos2d.h>

class OPWindow
{
public:
    
    static cocos2d::Size getWindowSizeInPoints();
    static cocos2d::Size getWindowSizeInPixels();
    
    static cocos2d::Rect getSafeArea(bool homeIndicator = true, bool notch = true);
};

#endif /* OPWindow_h */

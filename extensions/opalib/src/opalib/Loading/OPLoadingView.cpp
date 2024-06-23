//
//  OPLoadingView.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#include "OPLoadingView.hpp"

#include <cocos2d.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OPLoadingView_iOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OPLoadingView_Android.hpp"
#endif

OPLoadingView* OPLoadingView::shared()
{
    static OPLoadingView* instance = 0;
    
    if (!instance)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        instance = new OPLoadingView_iOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        instance = new OPLoadingView_Android();
#endif
    }
    
    return instance;
}

void OPLoadingView::show(const std::string& title)
{
    shared()->showView(title);
}

void OPLoadingView::hide()
{
    shared()->hideView();
}

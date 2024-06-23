//
//  OPLoadingView_iOS.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#include "OPLoadingView_iOS.hpp"
#include "OPLoadingView_iOSImpl.h"

#include <cocos2d.h>

OPLoadingView_iOS::OPLoadingView_iOS()
{
    impl = [[OPLoadingView_iOSImpl alloc] init];
}

void OPLoadingView_iOS::initialize()
{
    [((OPLoadingView_iOSImpl*)impl) setup];
}

void OPLoadingView_iOS::showView(const std::string& title)
{
    NSString* nsTitle = [NSString stringWithUTF8String:title.c_str()];
    [((OPLoadingView_iOSImpl*)impl) showWithTitle:nsTitle];
}

void OPLoadingView_iOS::hideView()
{
    [((OPLoadingView_iOSImpl*)impl) hide];
}

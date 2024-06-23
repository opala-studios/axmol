//
//  OPEvents.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/03/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPEvents_hpp
#define OPEvents_hpp

#include <stdio.h>

class OPEvents
{
public:
    struct ApplicationDidEnterBackground
    {
        
    };
    
    struct ApplicationWillEnterForeground
    {
        
    };
    
    static const char* applicationDidEnterBackground;
    static const char* applicationWillEnterForeground;
	static const char* applicationWillResignActive;
	static const char* applicationDidBecomeActive;
    static const char* bannerVisibilityDidChange;
};

#endif /* OPEvents_hpp */

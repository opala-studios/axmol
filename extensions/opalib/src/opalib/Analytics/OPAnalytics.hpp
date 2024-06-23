//
//  OPAnalytics.hpp
//  Opalib
//
//  Created by Adilson Tavares on 26/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPAnalytics_hpp
#define OPAnalytics_hpp

#include <iostream>

class OPAnalyticsEvent;

class OPAnalytics
{
public:
    
    static OPAnalytics* shared();
    
    void initialize();
    
    virtual void setup() = 0;
    virtual void sendEvent(const OPAnalyticsEvent& event);
    virtual void setUserProperty(const std::string& key, const std::string& value) = 0;

    void setCurrentScreen(const std::string& screenName);
};

#endif /* OPAnalytics_hpp */

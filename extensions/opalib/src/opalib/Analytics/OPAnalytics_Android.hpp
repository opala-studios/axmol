//
//  OPAnalytics_Android.hpp
//  Opalib
//
//  Created by Adilson Tavares on 29/07/19.
//  Copyright © 2019 Opala Stud_Android. All rights reserved.
//

#ifndef OPAnalytics_Android_hpp
#define OPAnalytics_Android_hpp

#include "OPAnalytics.hpp"

class OPAnalytics_Android : public OPAnalytics
{
public:
    
    OPAnalytics_Android();
    
    void setup() override;
    void sendEvent(const OPAnalyticsEvent& event) override;
    void setUserProperty(const std::string& key, const std::string& value) override;
};

#endif /* OPAnalytics_Android_hpp */

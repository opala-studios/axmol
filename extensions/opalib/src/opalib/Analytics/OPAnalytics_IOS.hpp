//
//  OPAnalyticsIOS.hpp
//  Opalib
//
//  Created by Adilson Tavares on 29/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPAnalyticsIOS_hpp
#define OPAnalyticsIOS_hpp

#include "OPAnalytics.hpp"

class OPAnalytics_IOS : public OPAnalytics
{
private:
    
    void* impl {0};
    

public:
    
    OPAnalytics_IOS();
    
    void setup() override;
    void sendEvent(const OPAnalyticsEvent& event) override;
    void setUserProperty(const std::string& key, const std::string& value) override;
};

#endif /* OPAnalyticsIOS_hpp */

//
//  OPAnalyticsEvent.hpp
//  Opalib
//
//  Created by Adilson Tavares on 26/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPAnalyticsEvent_hpp
#define OPAnalyticsEvent_hpp

#include <iostream>
#include <map>

#include "../Misc/OPParameter.hpp"

class OPAnalyticsEvent
{
public:
    
    typedef std::map<std::string, OPParameter> ParametersMap;
    
private:
    
    std::string name;
    ParametersMap parameters;
    
public:
    
    OPAnalyticsEvent(const std::string& name);
    
    const std::string& getName() const { return name; }
    const ParametersMap getParameters() const { return parameters; }
    
    OPAnalyticsEvent& set(const std::string& name, bool boolValue);
    OPAnalyticsEvent& set(const std::string& name, int intValue);
    OPAnalyticsEvent& set(const std::string& name, double doubleValue);
    OPAnalyticsEvent& set(const std::string& name, const char* stringValue);
    OPAnalyticsEvent& set(const std::string& name, const std::string& stringValue);
    
    void send() const;
};

#endif /* OPAnalyticsEvent_hpp */

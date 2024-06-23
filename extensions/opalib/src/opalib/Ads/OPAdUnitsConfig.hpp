//
//  ADConfig.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 01/07/19.
//

#ifndef ADConfig_hpp
#define ADConfig_hpp

#include <iostream>
#include <json/document.h>
#include "../Debug/OPDebug.hpp"

struct OPAdUnitsConfig
{    
    std::string appKey;
    
    std::string bannerId;
    std::string interstitialId;
    std::string rewardedId;
    
    OPAdUnitsConfig()
    {
    }
    
    OPAdUnitsConfig(const rapidjson::Value& root)
    {
        appKey = root["appKey"].GetString();
        bannerId = root["banner"].GetString();
        interstitialId = root["interstitial"].GetString();
        rewardedId = root["rewarded"].GetString();
    }
};

#endif /* ADConfig_hpp */

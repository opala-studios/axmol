//
//  OPAdsConfig.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#ifndef OPAdsConfig_hpp
#define OPAdsConfig_hpp

#include "./OPAdUnitsConfig.hpp"
#include "../Helpers/OPPlatform.hpp"
#include "../Debug/OPDebug.hpp"
#include "./OPBannerPosition.hpp"

struct OPAdsConfig
{
    OPAdUnitsConfig platformConfig;
    OPBannerPosition bannerPosition;

    bool userConsent {true};
    bool smartBanner {true};
    int reloadAdInterval {0};
    float rewardCallbackWait {0};
    
    OPAdsConfig()
    {
    }
    
    OPAdsConfig(const rapidjson::Value& root)
    {
        platformConfig = OPAdUnitsConfig(root["units"][OPPlatform::id()]);

        bannerPosition = OPBannerPosition(root);

        smartBanner = root["smartBanner"].GetBool();
        reloadAdInterval = root["reloadAdInterval"].GetInt();
        rewardCallbackWait = root["rewardCallbackWait"].GetFloat();
    }
};

#endif /* OPAdsConfig_hpp */

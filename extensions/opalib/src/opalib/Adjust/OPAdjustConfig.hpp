//
//  OPAdjustConfig.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 03/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPAdjustConfig_hpp
#define OPAdjustConfig_hpp

#include <json/document-wrapper.h>
#include <string>

class OPAdjustConfig
{
public:
    OPAdjustConfig(const rapidjson::Value& json);

    const std::string getToken() const;
    const std::string& getInAppPurchaseToken() const;

protected:
    std::string _token;
    std::string _inAppPurchaseToken;
};

#endif /* OPAdjustConfig_hpp */

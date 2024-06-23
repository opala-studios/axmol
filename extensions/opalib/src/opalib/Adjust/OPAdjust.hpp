//
//  OPAdjust.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 03/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPAdjust_hpp
#define OPAdjust_hpp
//#include <Adjust/Adjust2dx.h>

#include "./OPAdjustConfig.hpp"

class OPAdjust
{
public:
    static OPAdjust* shared();

    void initialize(bool idfaAuthorized);

//    static void adjustSessionSuccess(AdjustSessionSuccess2dx callback);
//    static void adjustSessionFailure(AdjustSessionFailure2dx callback);

    OPAdjustConfig* getConfig() const;

    const std::string getInAppPurchaseToken() const;

protected:
    OPAdjustConfig* _config{nullptr};
};

#endif /* OPAdjust_hpp */

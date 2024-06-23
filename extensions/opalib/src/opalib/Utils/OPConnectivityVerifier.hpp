//
//  OPConnectivityVerifier.hpp
//  opalib-ios
//
//  Created by Opala Studios on 03/11/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPConnectivityVerifier_hpp
#define OPConnectivityVerifier_hpp

class OPConnectivityVerifier
{
public:
    static bool isAirplaneModeOn();

    static bool isActiveInternetConnection();
};

#endif /* OPConnectivityVerifier_hpp */

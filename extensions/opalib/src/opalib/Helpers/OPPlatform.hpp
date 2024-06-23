//
//  OPPlatform.hpp
//  Opalib
//
//  Created by Adilson Tavares on 12/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPPlatform_hpp
#define OPPlatform_hpp

#include <iostream>

class OPPlatform
{
public:
    
    static const char* id();
    static const char* name();
    
    static int getSystemVersion();
    static int getVersionCode();
};

#endif /* OPPlatform_hpp */

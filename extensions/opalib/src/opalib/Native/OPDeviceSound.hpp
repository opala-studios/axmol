//
//  OPDeviceSound.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 01/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPDeviceSound_hpp
#define OPDeviceSound_hpp

#include <cocos2d.h>

class OPDeviceSound : public cocos2d::Ref
{
public:
    
    CREATE_FUNC(OPDeviceSound);
    
    virtual bool init();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _name, Name);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _filePath, FilePath);
};

#endif /* OPDeviceSound_hpp */

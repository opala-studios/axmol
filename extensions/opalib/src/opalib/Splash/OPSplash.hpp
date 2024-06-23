//
//  OPSplash.hpp
//  Opalib
//
//  Created by Adilson Tavares on 16/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPSplash_hpp
#define OPSplash_hpp

#include <cocos2d.h>

class OPSplash : public cocos2d::Scene
{
private:
    
    cocos2d::Scene* initialScene {0};
    
    CREATE_FUNC(OPSplash);
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    static OPSplash* create(cocos2d::Scene* initialScene);
};

#endif /* OPSplash_hpp */

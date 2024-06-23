//
//  OPGRoot.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPGRoot_hpp
#define OPGRoot_hpp

#include <FairyGUI.h>
#include <cocos2d.h>
//#include "../Audio/OPPlayer.hpp"


class OPGRoot : public fairygui::GRoot
{
public:
    
    void playSound(fairygui::PackageItem* item, float volumeScale) override;
    
    static GRoot* create(cocos2d::Scene* scene, int zOrder = 1000);
    
protected:
    
//    cocos2d::Vector<OPPlayer> _players;
};

#endif /* OPGRoot_hpp */

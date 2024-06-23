//
//  OPStreamedPlayer.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 09/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPStreamedPlayer_hpp
#define OPStreamedPlayer_hpp

#include "OPPlayer.hpp"

class OPStreamedPlayer: public OPPlayer
{
public:
    static std::unique_ptr<OPStreamedPlayer> createUnique(const std::string& filePath);
    static std::shared_ptr<OPStreamedPlayer> createShared(const std::string& filePath);

    OPStreamedPlayer(const std::string& filePath);

    void createImplementation() override;
    void destroyImplementation() override;

private:
    std::string _filepath;

};
#endif /* OPStreamedPlayer_hpp */

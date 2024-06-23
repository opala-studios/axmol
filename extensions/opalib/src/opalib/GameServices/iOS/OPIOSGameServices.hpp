//
//  OPIOSGameServices.hpp
//  opalib-ios
//
//  Created by Opala Teste on 11/02/22.
//  Copyright © 2022 Opala Studios. All rights reserved.
//

#ifndef OPIOSGameServices_hpp
#define OPIOSGameServices_hpp

#include <stdio.h>
#include <opalib/GameServices/OPGameServices.hpp>
#import <Foundation/Foundation.h>
#import <GameKit/GKSavedGame.h>

class OPIOSGameServices : public OPGameServices {
public:
    void initialize();

    void signIn(const SignInCallback& callback);

    void loadSnapshotMetadata(const LoadSnapshotMetadataCallback& callback);

    void saveSnapshotContent(const std::string &snapshotName, void* data, size_t size, const SaveSnapshotContentCallback& callback);

    void loadSnapshotContent(const std::string &snapshotName, const LoadSnapshotContentCallback& callback);
private:
    void checkLocalPlayer();
private:
    SignInCallback _signInCallback;
    NSMutableDictionary<NSString*, GKSavedGame*>* _savedGames;
    bool _signingIn = false;
    bool _initialized = false;
};

#endif /* OPIOSGameServices_hpp */

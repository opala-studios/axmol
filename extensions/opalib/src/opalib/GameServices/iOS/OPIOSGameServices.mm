//
//  OPIOSGameServices.cpp
//  opalib-ios
//
//  Created by Opala Teste on 11/02/22.
//  Copyright © 2022 Opala Studios. All rights reserved.
//

#include "OPIOSGameServices.hpp"
#include <GameKit/GKPlayer.h>
#include <GameKit/GKSession.h>
#include <GameKit/GKLocalPlayer.h>
#include <opalib/Debug/OPDebug.hpp>
#include <opalib/Application/OPAppController.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIViewController.h>
#import <GameKit/GKLocalPlayer.h>
#import <GameKit/GKSavedGame.h>


OPGameServices* OPGameServices::shared(){
    static OPIOSGameServices instance;
    return (OPGameServices*)&instance;
}

void OPIOSGameServices::initialize(){
    
}

void OPIOSGameServices::signIn(const SignInCallback& callback){
    _signInCallback = callback;
    GKLocalPlayer.local.authenticateHandler = ^(UIViewController* __nullable view, NSError* __nullable error){
        if (view != nil){
            OPAppController* app = (OPAppController*)[[UIApplication sharedApplication] delegate];
            [app.viewController presentViewController: view animated:true completion:^{
                OP_LOG("Authentication view completion called");
                checkLocalPlayer();
                return;
            }];
        }
        if (error != nil){
            OP_LOG("error " + std::string(error.localizedDescription.UTF8String));
            return;
        }
        checkLocalPlayer();
    };
}


void OPIOSGameServices::checkLocalPlayer(){
    if (GKLocalPlayer.local.authenticated){
        OP_LOG("Authenticated");
        _signedAccount = std::make_unique<Account>(GKLocalPlayer.local.playerID.UTF8String, GKLocalPlayer.local.displayName.UTF8String);
        _signInCallback(ResultCode::SUCCESS, _signedAccount.get());
    }
    else {
        OP_LOG("Not authenticated");
        _signedAccount.reset();
        _signInCallback(ResultCode::FAILED_UNKNOWN, nullptr);
    }
}

void OPIOSGameServices::loadSnapshotMetadata(const LoadSnapshotMetadataCallback& callback){
    if (!_signedAccount){
        callback(ResultCode::FAILED_NOT_SIGNED_IN);
        return;
    }
    
    auto localCallback = callback;
    
    [GKLocalPlayer.local fetchSavedGamesWithCompletionHandler:^(NSArray<GKSavedGame *> * _Nullable savedGames, NSError * _Nullable error) {
     if (error != nil){
        localCallback(ResultCode::FAILED_UNKNOWN);
        return;
     }
     _savedGames = [[NSMutableDictionary<NSString*, GKSavedGame*> alloc] init];
     
     for (GKSavedGame* savedGame in savedGames){
        [_savedGames setObject:savedGame forKey:savedGame.name];
        _signedAccount->addSnapshot(savedGame.name.UTF8String, (long)savedGame.modificationDate.timeIntervalSince1970);
     }
     localCallback(ResultCode::SUCCESS);
     }];
}

void OPIOSGameServices::saveSnapshotContent(const std::string &snapshotName, void* data, size_t size, const SaveSnapshotContentCallback& callback){
    if (!_signedAccount){
        callback(ResultCode::FAILED_NOT_SIGNED_IN);
        return;
    }
    NSData* d = [NSData dataWithBytes:data length:size];
    [GKLocalPlayer.local saveGameData:d withName:[NSString stringWithUTF8String:snapshotName.c_str()] completionHandler:^(GKSavedGame * _Nullable savedGame, NSError * _Nullable error) {
     if (error != nil){
        OP_LOG("Error when saving snapshot content: " + std::string(error.localizedDescription.UTF8String));
        callback(ResultCode::FAILED_UNKNOWN);
        return;
     }
     if (savedGame == nil){
        callback(ResultCode::FAILED_UNKNOWN);
        return;
     }
     callback(ResultCode::SUCCESS);
     }];
}

void OPIOSGameServices::loadSnapshotContent(const std::string &snapshotName, const LoadSnapshotContentCallback& callback){
    if (!_signedAccount){
        callback(ResultCode::FAILED_NOT_SIGNED_IN, nullptr, 0);
        return;
    }
    NSString* snapshotKey = [NSString stringWithUTF8String:snapshotName.c_str()];
    GKSavedGame* savedGame = _savedGames[snapshotKey];
    if (savedGame == nil){
        callback(ResultCode::FAILED_SNAPSHOT_NAME_NOT_FOUND, nullptr, 0);
        return;
    }
    
    auto localCallback = callback;
    
    [savedGame loadDataWithCompletionHandler:^(NSData * _Nullable data, NSError * _Nullable error) {
     if (error != nil){
        localCallback(ResultCode::FAILED_UNKNOWN, nullptr, 0);
        return;
     }
     size_t size = [data length];
     void* ownedData = malloc(size);
     memcpy(ownedData, data.bytes, size);
     localCallback(ResultCode::SUCCESS, ownedData, size);
     free(ownedData);
     }];
}

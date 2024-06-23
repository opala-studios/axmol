//
//  OPRewarded.m
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import "OPRewarded.h"
#import "OPCocosThread.hpp"
#import "OPDebug.hpp"
#include "../../Misc/OPEventBus.hpp"

@implementation OPRewarded

- (instancetype)initWithConfig:(OPAdsConfig *)config
{
    self = [super init];
    if (self)
    {
        _config = config;
    }
    
    return self;
}

- (void)debugLog:(NSString*)message
{
    OPDebug::log("ADS | Rewarded", message.UTF8String);
}

- (void)setup
{
    [IronSource setRewardedVideoDelegate:self];
}

- (void)show:(const OPAds::SEL_RewardedHandlerWithCode&)completion
{
    [self debugLog:@"Request show"];
    
    if (!self.isReady)
    {
        [self debugLog:@"Not shown because it is not ready"];
    }
    
    _closed = false;
    _containsReward = false;
    _showing = false;
    _completionInvokeId = -1;
    _completion = completion;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        UIViewController* rootViewController = UIApplication.sharedApplication.keyWindow.rootViewController;
        [IronSource showRewardedVideoWithViewController:rootViewController];
    });
    
}

- (BOOL)isReady
{
    return [IronSource hasRewardedVideo];
}

- (void)callCompletion:(int)withCode
{
    if (!_completion) {
        return;
    }
    
    if (_completionInvokeId >= 0)
    {
        OPCocosThread::cancelInvoke(_completionInvokeId);
        _completionInvokeId = -1;
    }
    
    OPCocosThread::invoke("rewarded_call_completion", [self, withCode]()
    {
        [self debugLog:[NSString stringWithFormat:@"Call completion (reward = %d)", _containsReward]];

        if (_completion)
        {
//            _completion(_containsReward? -1 : -2);
            _completion(withCode);
            _completion = NULL;
        }
    });
}

// MARK: - ISRewardedDelegate

- (void)rewardedVideoDidStart
{
    [self debugLog:@"Start"];
    //_completion(0);
}

- (void)rewardedVideoDidEnd
{
    [self debugLog:@"End"];
}

- (void)rewardedVideoDidOpen
{
    [self debugLog:@"Open"];
    _showing = true;
    _completion(0);
}

- (void)rewardedVideoDidClose
{
    [self debugLog:@"Close"];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        _closed = true;
      
        if (_containsReward) {
            [self callCompletion:-1];
        }
        else
        {
            [self debugLog:[NSString stringWithFormat:@"Rewarded not received yet. Waiting %fs", _config->rewardCallbackWait]];
            
            _completionInvokeId = OPCocosThread::invoke("rewarded_call_completion_delayed", _config->rewardCallbackWait, [self] {
                [self debugLog:@"Delayed completion called"];
                [self callCompletion:-2];
            });
        }
    });
}

- (void)rewardedVideoDidFailToShowWithError:(NSError *)error
{
    [self debugLog:@"Failed to show"];
    
    // Se não tiver código válido define statusCode como -1000 (FAILED_UNKNOWN)
    int code = (error)? error.code : -1000;
    // Se tiver mostrando um AD e
    // ocorrer uma falha que não seja código (1022 - já mostrando um AD)
    // Define código personalizado
    if (_showing && code != 1022)
        code = -100; // (SHOWED_BUT_FAILED)
    // Mesmo não tendo código de erro com valor ZERO,
    // IronSource retorna erro com código 0 e com mensagem estranha
    else if (code == 0)
        code = -1000; // (FAILED_UNKNOWN)
    
    [self callCompletion:code];
}

- (void)rewardedVideoHasChangedAvailability:(BOOL)available
{
    [self debugLog:[NSString stringWithFormat:@"Change availability: %d", available]];
}

- (void)didClickRewardedVideo:(ISPlacementInfo *)placementInfo
{
    [self debugLog:@"Click"];
}

- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo
{
    [self debugLog:@"Reward received"];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        _containsReward = true;
        
        if (_closed) {
            [self callCompletion];
        }
    });
}

@end

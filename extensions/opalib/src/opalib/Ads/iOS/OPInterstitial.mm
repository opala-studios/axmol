//
//  OPInterstitial.m
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import "OPInterstitial.h"
#import "OPCocosThread.hpp"
#import "OPDebug.hpp"

@implementation OPInterstitial

- (instancetype)initWithConfig:(OPAdsConfig*)config
{
    self = [super init];
    if (self)
    {
        _config = config;
        _lastShowDate = [[NSDate alloc] initWithTimeIntervalSince1970:0];
        _showing = false;
    }
    
    return self;
}

- (void)debugLog:(NSString*)message
{
    OPDebug::log("ADS | Interstitial", message.UTF8String);
}

- (void)setup
{
    [IronSource setInterstitialDelegate:self];
    
    [self debugLog:@"Initialize"];
    [self load];
}

- (void)load
{
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0ul);
    dispatch_async(queue, ^{
        [self debugLog:@"Request load"];
        [IronSource loadInterstitial];
    });
}

- (void)show:(const OPAds::SEL_InterstitialHandlerWithCode&)completion
{
    [self debugLog:@"Request show"];
    
    if (!self.isReady)
    {
        [self debugLog:@"Not showing because it is not ready"];
    }
    
    _showing = false;
    _completion = completion;
    
    UIViewController* rootViewController = UIApplication.sharedApplication.keyWindow.rootViewController;
    [IronSource showInterstitialWithViewController:rootViewController];
}

- (BOOL)isReady
{
    return [IronSource hasInterstitial];
}

// MARK: - ISInterstitialDelegate

- (void)interstitialDidLoad
{
    [self debugLog:@"Loaded"];
    OPCocosThread::invoke("interstitial_loaded", []() {
        OPAds::shared()->onInterstitialLoaded();
    });
}

- (void)interstitialDidShow
{
    [self debugLog:@"Did show"];
    _lastShowDate = [[NSDate alloc] init];
}

- (void)interstitialDidOpen
{
    [self debugLog:@"Open"];
    _showing = true;
    _completion(0);
}

- (void)interstitialDidClose
{
    [self debugLog:@"Closed"];
    
    
    [self callCompletion:-1];
    [self load];
    _showing = false;
}

- (void)scheduleReload
{
    [self debugLog:[NSString stringWithFormat:@"Reload schedule for %ds from now", _config->reloadAdInterval]];
    
    OPCocosThread::invoke("interstitial_schedule_reload", _config->reloadAdInterval, [self]() {
        [self load];
    });
}

- (void)interstitialDidFailToLoadWithError:(NSError *)error
{
    [self debugLog:@"Failed to load"];
    [self scheduleReload];
}

- (void)interstitialDidFailToShowWithError:(NSError *)error
{
    [self debugLog:@"Failed to show"];
    
    // Se não tiver código válido define statusCode como -1000 (FAILED_UNKNOWN)
    int code = (error)? (int)error.code : -1000;
    // Se tiver mostrando um AD e
    // ocorrer uma falha que não seja código (1036 - já mostrando um AD)
    // Define código personalizado
    if (_showing && code != 1036)
        code = -100;// (SHOWED_BUT_FAILED)
    // Mesmo não tendo código de erro com valor ZERO,
    // IronSource retorna erro com código 0 e com mensagem estranha
    else if (code == 0)
        code = -1000;// (FAILED_UNKNOWN)
    
    [self callCompletion:code];
    [self scheduleReload];
}

- (void)callCompletion:(int)withCode
{
    OPCocosThread::invoke("interstitial_call_completion", [self, withCode]()
    {
        [self debugLog:@"Call completion"];
        
        if (_completion)
        {
            _completion(withCode);
            //_completion = NULL;
        }
    });
}

- (void)didClickInterstitial
{
    [self debugLog:@"Click"];
}

@end

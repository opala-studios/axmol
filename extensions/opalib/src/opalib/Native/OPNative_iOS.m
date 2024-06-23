//
//  OPNative_iOS.m
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 27/01/20.
//

#import "OPNative_iOS.h"
#import "../iOS/OPOpalibViewController.h"
#import "../iOS/OPOpalibWindow.h"

@implementation OPNative_iOS

+ (instancetype)shared
{
    static OPNative_iOS* instance = 0;
    if (!instance) {
        instance = [[OPNative_iOS alloc] init];
    }
    return instance;
}

- (void)setup
{
    OPOpalibViewController* vc = [[OPOpalibViewController alloc] init];
    
    _opalibWindow = [[OPOpalibWindow alloc] init];
    _opalibWindow.backgroundColor = nil;
    _opalibWindow.rootViewController = vc;
    _opalibWindow.hidden = NO;
}

@end

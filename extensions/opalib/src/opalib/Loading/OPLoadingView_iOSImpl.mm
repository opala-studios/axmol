//
//  OPLoadingView_iOSImpl.m
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#import "OPLoadingView_iOSImpl.h"
#include <cocos2d.h>

@implementation OPLoadingView_iOSImpl

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        loading = false;
    }
    
    return self;
}

- (void)setup
{
    UIViewController* rootVC = UIApplication.sharedApplication.keyWindow.rootViewController;
    UIView* rootView = rootVC.view;
    
    loadingView = [[UIView alloc] init];
    loadingView.alpha = 0;
    loadingView.userInteractionEnabled = false;
    loadingView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.8];
    loadingView.translatesAutoresizingMaskIntoConstraints = false;
 
    [rootView addSubview:loadingView];
    [rootView.leftAnchor constraintEqualToAnchor:loadingView.leftAnchor].active = true;
    [rootView.rightAnchor constraintEqualToAnchor:loadingView.rightAnchor].active = true;
    [rootView.topAnchor constraintEqualToAnchor:loadingView.topAnchor].active = true;
    [rootView.bottomAnchor constraintEqualToAnchor:loadingView.bottomAnchor].active = true;

    UIBlurEffect* effect = [UIBlurEffect effectWithStyle:UIBlurEffectStyleDark];
    effectView = [[UIVisualEffectView alloc] initWithEffect:effect];
    effectView.translatesAutoresizingMaskIntoConstraints = false;

    [loadingView addSubview:effectView];
    [loadingView.leftAnchor constraintEqualToAnchor:effectView.leftAnchor].active = true;
    [loadingView.rightAnchor constraintEqualToAnchor:effectView.rightAnchor].active = true;
    [loadingView.topAnchor constraintEqualToAnchor:effectView.topAnchor].active = true;
    [loadingView.bottomAnchor constraintEqualToAnchor:effectView.bottomAnchor].active = true;

    indicatorView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    indicatorView.translatesAutoresizingMaskIntoConstraints = false;
    indicatorView.hidesWhenStopped = false;
    [indicatorView startAnimating];

    [effectView.contentView addSubview:indicatorView];
    [effectView.contentView.centerXAnchor constraintEqualToAnchor:indicatorView.centerXAnchor].active = true;
    [effectView.contentView.centerYAnchor constraintEqualToAnchor:indicatorView.centerYAnchor].active = true;

    label = [[UILabel alloc] init];
    label.translatesAutoresizingMaskIntoConstraints = false;
    label.text = @"";
    label.textColor = [UIColor whiteColor];
    label.alpha = 0.6;
    label.font = [UIFont fontWithName:@"UniSansHeavy" size:19];
    label.textAlignment = NSTextAlignmentCenter;

    [effectView.contentView addSubview:label];
    [label.leftAnchor constraintEqualToAnchor:effectView.contentView.leftAnchor constant:100].active = true;
    [effectView.contentView.rightAnchor constraintEqualToAnchor:label.rightAnchor constant:100].active = true;
    [effectView.contentView.bottomAnchor constraintEqualToAnchor:label.bottomAnchor constant:60].active = true;
}

- (void)showWithTitle:(NSString *)title
{
    if (loading) { return; }
    loading = true;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        self->loadingView.userInteractionEnabled = true;
        self->label.text = title;
        
        [UIView animateWithDuration:0.3 animations:^{
            self->loadingView.alpha = (self->loading ? 1 : 0);
        }];
    });
}

- (void)hide
{
    if (!loading) { return; }
    loading = false;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        self->loadingView.userInteractionEnabled = false;
        
        [UIView animateWithDuration:0.3 animations:^{
            self->loadingView.alpha = 0;
        }];
    });
}

@end

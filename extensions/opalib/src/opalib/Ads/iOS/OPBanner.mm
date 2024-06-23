//
//  OPBanner.m
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import "OPBanner.h"
#import "OPCocosThread.hpp"
#import "OPDebug.hpp"
#import "OPAds.hpp"
#import <IronSource/IronSource.h>

@implementation OPBanner
{
    NSMutableArray<NSLayoutConstraint*>* _bannerConstraints;
    NSMutableArray<NSLayoutConstraint*>* _backgroundConstraints;
}

- (void)debugLog:(NSString*)message
{
    OPDebug::log("ADS | Banner", message.UTF8String);
}

- (instancetype)initWithConfig:(OPAdsConfig*)config
{
    self = [super init];
    if (self)
    {
        _config = config;
        _bannerPosition = config->bannerPosition;
    }
    
    return self;
}

- (void)setup
{
    _bannerConstraints = [[NSMutableArray alloc] init];
    _backgroundConstraints = [[NSMutableArray alloc] init];
    
    [self debugLog:@"Initialize"];

    _background = [[UIView alloc] init];
    _background.translatesAutoresizingMaskIntoConstraints = NO;
    _background.backgroundColor = UIColor.clearColor;

    [IronSource setBannerDelegate:self];
    [self load];
}

- (float)preferredBannerHeight
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && _config->smartBanner) {
        return 90;
    }
    else {
        return 50;
    }
}

- (float)preferredBannerWidth
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && _config->smartBanner) {
        return 728;
    }
    else {
        return 320;
    }
}

- (float)height
{
//    float safeOffset = 0;
//
//    if (@available(iOS 11.0, *))
//    {
//        UIWindow* window = UIApplication.sharedApplication.keyWindow;
//        if (_bannerPosition.vertical == OPBannerPosition::Vertical::Bottom) {
//            safeOffset = window.safeAreaInsets.bottom;
//        }
//        else {
//            safeOffset = window.safeAreaInsets.top;
//        }
//    }

//    return self.preferredBannerHeight + safeOffset;
    return [self preferredBannerHeight];

}

- (float)width
{
    return [self preferredBannerWidth];
}


- (void)show:(OPBannerPosition)position
{
    [self debugLog:@"Request show"];
    
    if (position != _bannerPosition)
    {
        _needsConstraintsUpdate = YES;
    
        [self debugLog:[NSString stringWithFormat:@"Show was requested for a different location (%s => %s)",
                                                          _bannerPosition.getName().c_str(),
                                                          position.getName().c_str()]];
        
        _bannerPosition = position;
        [self updateBannerViewConstraints];
    }
    
    _wantsBanner = YES;
    [self updateVisibility];
}

- (void)hide
{
    [self debugLog:@"Request hide"];
    
    _background.hidden = YES;
    _wantsBanner = NO;
    [self updateVisibility];
}

- (void)setBackgroundColor:(const cocos2d::Color4B&)color
{
    _background.backgroundColor = [UIColor colorWithRed:(color.r / 255.0) green:(color.g / 255.0) blue:(color.b / 255.0) alpha:(color.a / 255.0)];
}

- (void)updateConstraintsIfNeeded
{
    if (_bannerView != nil && _needsConstraintsUpdate) {
        [self updateBannerViewConstraints];
    }
}

- (void)updateBannerViewConstraints
{
    if (_bannerView == nil){
        return;
    }
    
    [self debugLog:[NSString stringWithFormat:@"Updating view constraints to %s", _bannerPosition.getName().c_str()]];
    _needsConstraintsUpdate = NO;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        UIViewController* vc = UIApplication.sharedApplication.keyWindow.rootViewController;
        UIView* mainView = vc.view;
        
        [self clearConstraints];
        
        if (_background.superview == nil) {
            [mainView addSubview:_background];
        }
        
        if (_bannerView.superview == nil)
        {
            _bannerView.translatesAutoresizingMaskIntoConstraints = NO;
            [_background addSubview:_bannerView];
        }
        
        UILayoutGuide *guide;
        
        bool isSafeLayout = false;
        
        if (@available(iOS 11.0, *)) {
            guide = [mainView safeAreaLayoutGuide];
            isSafeLayout = true;
        }
        else {
            guide = [mainView layoutMarginsGuide];
            isSafeLayout = false;
        }
        
        //first we set the backgroud constrains to be the width of screen and height of preferred banner size centered horizontally
        [_backgroundConstraints addObject:[_background.heightAnchor constraintEqualToConstant:[self preferredBannerHeight]]];
        [_backgroundConstraints addObject: [_background.widthAnchor constraintEqualToConstant: mainView.frame.size.width]];
        
        [_backgroundConstraints addObject: [_background.centerXAnchor constraintEqualToAnchor:mainView.centerXAnchor]];
        
        //make the banner view follow the dummy background vertically
        [_bannerConstraints addObject: [_bannerView.topAnchor constraintEqualToAnchor:_background.topAnchor]];
        [_bannerConstraints addObject: [_bannerView.bottomAnchor constraintEqualToAnchor:_background.bottomAnchor]];
        
        //set the size constrains of banner
        [_bannerConstraints addObject:[_bannerView.widthAnchor constraintEqualToConstant: _bannerView.bounds.size.width]];
        [_bannerConstraints addObject:[_bannerView.heightAnchor constraintEqualToConstant:[self preferredBannerHeight]]];
        
        //now based on the strategy we place the banner on the left - center - or rigth taking in consideration the ISBannerView's size
        //cause it is different from the preferred size asket to it (normally 768 of width - thats is why we used the centerX anchor)
        switch (_bannerPosition.vertical ) {
            case OPBannerPosition::Vertical::Top:
                [_backgroundConstraints addObject: [_background.topAnchor constraintEqualToAnchor:guide.topAnchor]];
                break;
            case OPBannerPosition::Vertical::Bottom:
                [_backgroundConstraints addObject: [_background.bottomAnchor constraintEqualToAnchor:guide.bottomAnchor]];
                break;
            default:
                break;
        }

        switch (_bannerPosition.horizontal ) {
            case OPBannerPosition::Horizontal::Left:
                if(isSafeLayout)
                {
                    [_bannerConstraints addObject: [_bannerView.centerXAnchor constraintEqualToAnchor:guide.leftAnchor constant:[self preferredBannerWidth]/2]];
                }
                else
                {
                    [_bannerConstraints addObject: [_bannerView.centerXAnchor constraintEqualToAnchor:_background.leftAnchor constant:[self preferredBannerWidth]/2]];
                }
            case OPBannerPosition::Horizontal::Center:
                [_bannerConstraints addObject: [_bannerView.centerXAnchor constraintEqualToAnchor:_background.centerXAnchor]];
                break;
            case OPBannerPosition::Horizontal::Right:
                if(isSafeLayout)
                {
                    [_bannerConstraints addObject: [_bannerView.centerXAnchor constraintEqualToAnchor:guide.rightAnchor constant:-[self preferredBannerWidth]/2]];
                }
                else
                {
                    [_bannerConstraints addObject: [_bannerView.centerXAnchor constraintEqualToAnchor:_background.rightAnchor constant:-[self preferredBannerWidth]/2]];
                }
               
                break;
            default:
                break;
        }
        
        for (NSLayoutConstraint* constraint : _bannerConstraints) {
            [constraint setActive:YES];
        }

        for (NSLayoutConstraint* constraint : _backgroundConstraints) {
            [constraint setActive:YES];
        }

        [mainView layoutIfNeeded];
    });
}

- (void)clearConstraints
{
    UIViewController* vc = UIApplication.sharedApplication.keyWindow.rootViewController;
    UIView* mainView = vc.view;

    [mainView removeConstraints:_backgroundConstraints];
    [mainView removeConstraints:_bannerConstraints];
    [_background removeConstraints:_backgroundConstraints];
    [_bannerView removeConstraints:_bannerConstraints];
    
    [_backgroundConstraints removeAllObjects];
    [_bannerConstraints removeAllObjects];
}

- (void)updateVisibility
{
    [self debugLog:[NSString stringWithFormat:@"Update visibility: %d", _wantsBanner]];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        _background.hidden = !_wantsBanner;
        
        if (_bannerView != nil) {
            [_bannerView setHidden:!_wantsBanner];
        }
    });
}

- (void)load
{
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0ul);
    dispatch_async(queue, ^{

        if (@available(iOS 9.0, *)) {
        } else {
            // dont show banner in old ios
            return;
        }
        if (_isLoading) {
            return;
        }
        
        [self debugLog:@"Request load"];
        
        _isLoading = true;
        
        UIViewController* rootViewController = UIApplication.sharedApplication.keyWindow.rootViewController;
        
        ISBannerSize* size = _config->smartBanner ? ISBannerSize_SMART : ISBannerSize_BANNER;
        
        [IronSource loadBannerWithViewController:rootViewController size:size];

    });
}

- (BOOL)isVisible
{
    return _bannerView != nil && !_bannerView.hidden;
}

// MARK: - ISBannerDelegate

- (void)bannerWillLeaveApplication
{
    [self debugLog:@"Will leave application"];
}

- (void)bannerDidDismissScreen
{
    [self debugLog:@"Did dismiss screen"];
}

- (void)bannerWillPresentScreen
{
    [self debugLog:@"Will present screen"];
}

- (void)bannerDidFailToLoadWithError:(NSError *)error
{
    _isLoading = false;
    
    [self clearConstraints];
    
    if (_bannerView && _bannerView.superview) {
        [_bannerView removeFromSuperview];
    }
    
    if (_background.superview) {
        [_background removeFromSuperview];
    }
    
    _bannerView = nil;
    
    [self debugLog:[NSString stringWithFormat:@"Failed to load: %@", error]];
    [self debugLog:[NSString stringWithFormat:@"Waiting %ds to try again", _config->reloadAdInterval]];
    
//    OPCocosThread::invoke("banner_fail_reload_interval", _config->reloadAdInterval, [self]() {
    [self load];
//    });
}

- (void)bannerDidLoad:(ISBannerView *)bannerView
{
    [self debugLog:@"Loaded successfully"];
    
    _bannerView = bannerView;
    _bannerView.hidden = YES;
    _isLoading = false;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [self updateBannerViewConstraints];
        [self updateVisibility];
        OPAds::shared()->dispatchBannerVisibilityChangeEvent();
    });
}

- (BOOL)isReady
{
    return _bannerView != nil;
}

- (void)didClickBanner
{
    [self debugLog:@"Clicked"];
}

@end

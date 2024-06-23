//
//  OPDeviceSounds_iOSImpl.m
//  opalib-ios
//
//  Created by Adilson Tavares on 01/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#import "OPDeviceSounds_iOSImpl.h"

@implementation OPDeviceSounds_iOSImpl

- (instancetype)init
{
    if (self = [super init])
    {
        _showing = NO;
        _completion = nil;
    }
    
    return self;
}

- (void)finishWithURL:(NSURL*)url
{
    if (_completion)
    {
        _completion(url);
        _completion = nil;
    }
    
    _showing = NO;
}

- (void)documentBrowser:(UIDocumentBrowserViewController *)controller didPickDocumentURLs:(NSArray<NSURL *> *)documentURLs API_AVAILABLE(ios(11.0))
{
    NSURL* url = documentURLs != NULL ? documentURLs.firstObject : NULL;
    [controller dismissViewControllerAnimated:YES completion:nil];
    [self finishWithURL:url];
}

- (void)dismiss
{
    [UIApplication.sharedApplication.keyWindow.rootViewController.presentedViewController dismissViewControllerAnimated:YES completion:nil];
    [self finishWithURL:nil];
}

- (void)showWithCompletion:(nullable void(^)(NSURL*))completion
{
    if (_showing) { return; }
    _showing = YES;
    
    self.completion = completion;
    
    if (@available(iOS 11.0, *))
    {
        UINavigationController* nav = [[UINavigationController alloc] init];
        nav.navigationBarHidden = YES;
        UIBarButtonItem* cancelButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(dismiss)];
        
        NSArray* contentTypes = [NSArray arrayWithObject:@"public.audio"];
        UIDocumentBrowserViewController* browser = [[UIDocumentBrowserViewController alloc] initForOpeningFilesWithContentTypes:contentTypes];
        browser.allowsDocumentCreation = NO;
        browser.allowsPickingMultipleItems = NO;
        browser.delegate = self;
        browser.additionalTrailingNavigationBarButtonItems = [NSArray arrayWithObject:cancelButton];
        nav.viewControllers = [NSArray arrayWithObject:browser];
        
        [UIApplication.sharedApplication.keyWindow.rootViewController presentViewController:nav animated:YES completion:nil];
    }
}

@end


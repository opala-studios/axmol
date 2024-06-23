//
//  OPShare.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 07/04/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "../OPShare.hpp"
#include "../../Debug/OPDebug.hpp"
#include <FairyGUI.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


void OPShare::shareFile(const char* filePath, cocos2d::Rect rect){

    NSString *string = [[NSString alloc] initWithUTF8String:filePath];
    NSURL *URL = [[NSURL alloc] initFileURLWithPath:string];

    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:@[URL]
                                    applicationActivities:nil];
    UIViewController *rootViewController = UIApplication.sharedApplication.keyWindow.rootViewController;
    activityViewController.popoverPresentationController.sourceView = rootViewController.view;
    
    activityViewController.popoverPresentationController.sourceRect = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    activityViewController.popoverPresentationController.permittedArrowDirections = UIPopoverArrowDirectionAny;
    
    [rootViewController presentViewController:activityViewController
                                      animated:YES
                                    completion:nil];
}


void OPShare::shareMessage(const char* message, cocos2d::Rect rect){
    
    NSString *string = [[NSString alloc] initWithUTF8String:message];
    
    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:@[string]
                                    applicationActivities:nil];
    UIViewController *rootViewController = UIApplication.sharedApplication.keyWindow.rootViewController;
    activityViewController.popoverPresentationController.sourceView = rootViewController.view;
    activityViewController.popoverPresentationController.permittedArrowDirections = UIPopoverArrowDirectionAny;

    [rootViewController presentViewController:activityViewController
                                      animated:YES
                                    completion:nil];
}


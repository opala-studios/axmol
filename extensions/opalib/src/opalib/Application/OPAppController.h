//
//  OPAppController.h
//  opalib-ios
//
//  Created by Opala Teste on 03/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPAppController_h
#define OPAppController_h

#import <UIKit/UIKit.h>

@class RootViewController;

@interface OPAppController : NSObject <UIApplicationDelegate> {

}

@property(nonatomic, readwrite) RootViewController* viewController;

@end

#endif /* OPAppController_h */

//
//  OPDeviceSounds_iOSImpl.h
//  opalib-ios
//
//  Created by Adilson Tavares on 01/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MediaPlayer/MediaPlayer.h>

NS_ASSUME_NONNULL_BEGIN

@interface OPDeviceSounds_iOSImpl : NSObject <UIDocumentBrowserViewControllerDelegate>
{
    BOOL _showing;
}

@property (nonatomic, copy) void(^completion)(NSURL*);

- (void)showWithCompletion:(nullable void(^)(NSURL*))completion;

@end

NS_ASSUME_NONNULL_END

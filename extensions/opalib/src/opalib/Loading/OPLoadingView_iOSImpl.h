//
//  OPLoadingView_iOSImpl.h
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface OPLoadingView_iOSImpl : NSObject
{
    UIVisualEffectView* effectView;
    UIView* loadingView;
    UIActivityIndicatorView* indicatorView;
    UILabel* label;
    
    BOOL loading;
}

- (void)setup;
- (void)showWithTitle:(NSString*)title;
- (void)hide;

@end

NS_ASSUME_NONNULL_END

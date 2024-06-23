//
//  OPOpalibWindow.m
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 27/01/20.
//

#import "OPOpalibWindow.h"

@implementation OPOpalibWindow

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    if (self.rootViewController == nil) {
        return nil;
    }
    
    UIView* view = [super hitTest:point withEvent:event];
    
    if (view == self.rootViewController.view) {
        return nil;
    }
    
    return view;
}

@end

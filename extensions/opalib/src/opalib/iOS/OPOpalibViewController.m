//
//  OPOpalibViewController.m
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 27/01/20.
//

#import "OPOpalibViewController.h"

@implementation OPOpalibViewController

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

// HOME Indicator need to be tapped twice
-(UIRectEdge)preferredScreenEdgesDeferringSystemGestures
{
    return UIRectEdgeAll;
}

@end

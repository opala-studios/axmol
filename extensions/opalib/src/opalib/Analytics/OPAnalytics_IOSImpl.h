//
//  OPAnalyticsIOSImpl.h
//  Opalib
//
//  Created by Adilson Tavares on 29/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OPAnalyticsEvent.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface OPAnalytics_IOSImpl : NSObject
{
}

- (void)setup;
- (void)sendEvent:(const OPAnalyticsEvent&)event;
- (void)setUserProperty:(const std::string&)key withValue:(const std::string&)value;

@end

NS_ASSUME_NONNULL_END

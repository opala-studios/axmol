//
//  OPNativeAlert_iOS.mm
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 01/08/19.
//

#include "OPNativeAlert_iOS.hpp"
#include "../Misc/OPCocosThread.hpp"

#import <UIKit/UIKit.h>

OPNativeAlert_iOS::OPNativeAlert_iOS() : OPNativeAlert()
{
}

void OPNativeAlert_iOS::setup()
{
    
}

void OPNativeAlert_iOS::show(const std::string &title, const std::string &message, const std::string& buttonText, SEL_AlertCompletion completion)
{
    NSString* stitle = [NSString stringWithUTF8String:title.c_str()];
    NSString* smessage = [NSString stringWithUTF8String:message.c_str()];
    NSString* sbutton = [NSString stringWithUTF8String:buttonText.c_str()];
    
    UIAlertController* controller = [UIAlertController alertControllerWithTitle:stitle message:smessage preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction* dismiss = [UIAlertAction actionWithTitle:sbutton style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
        if (completion) {
            OPCocosThread::invoke("native_alert_dismiss", completion);
        }
    }];
    
    [controller addAction:dismiss];
    
    UIViewController* root = UIApplication.sharedApplication.keyWindow.rootViewController;
    [root presentViewController:controller animated:YES completion:nil];
}

//
//  OPNativeAlert.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 01/08/19.
//

#ifndef OPNativeAlert_hpp
#define OPNativeAlert_hpp

#include <iostream>
#include <stdio.h>
#include <cocos2d.h>

class OPNativeAlert
{
private:

    bool initialized {false};

protected:

    OPNativeAlert();
    
    virtual void setup() = 0;

public:
    
    typedef std::function<void()> SEL_AlertCompletion;

    void initialize();
    static OPNativeAlert* shared();
    
    void show(const std::string& title, const std::string& message, SEL_AlertCompletion completion = 0);
    virtual void show(const std::string& title, const std::string& message, const std::string& buttonText, SEL_AlertCompletion completion = 0) = 0;
};

#endif /* OPNativeAlert_hpp */

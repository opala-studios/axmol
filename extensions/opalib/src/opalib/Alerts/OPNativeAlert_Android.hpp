//
//  OPNativeAlert_Android.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 01/08/19.
//

#ifndef OPNativeAlert_Android_hpp
#define OPNativeAlert_Android_hpp

#include "OPNativeAlert.hpp"

class OPNativeAlert_Android : public OPNativeAlert
{
protected:

    virtual void setup() override;

public:

    OPNativeAlert_Android();

    virtual void show(const std::string& title, const std::string& message, const std::string& buttonText, SEL_AlertCompletion completion = 0) override;
};

#endif /* OPOPNativeAlert_Android_hpp */

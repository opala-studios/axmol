//
//  OPNativeAlert_iOS.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 01/08/19.
//

#ifndef OPNativeAlert_iOS_hpp
#define OPNativeAlert_iOS_hpp

#include "OPNativeAlert.hpp"

class OPNativeAlert_iOS : public OPNativeAlert
{
protected:

    virtual void setup() override;

public:

    OPNativeAlert_iOS();
    
    virtual void show(const std::string &title, const std::string &message, const std::string& buttonText, SEL_AlertCompletion completion) override;
};

#endif /* OPOPNativeAlert_iOS_hpp */

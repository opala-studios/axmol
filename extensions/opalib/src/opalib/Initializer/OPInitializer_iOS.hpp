//
//  OPInitializer_iOS.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#ifndef OPInitializer_iOS_hpp
#define OPInitializer_iOS_hpp

#include "./OPInitializer.hpp"

class OPInitializer_iOS : public OPInitializer
{
public:
    
    virtual void initialize() override;
    bool needsIDFAConfirmation() override;
    void askForIDFAConfirmation(const std::function<void(bool)>& onCompletion) override;
};

#endif /* OPInitializer_iOS_hpp */

//
//  OPInitializer.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#ifndef OPInitializer_hpp
#define OPInitializer_hpp

#include <stdio.h>
#include <vector>

class OPInitializer
{
public:
    
    typedef std::function<void()> SEL_InitializationHandler;
    
    static OPInitializer* shared();
    
    virtual void initialize();
    virtual bool needsIDFAConfirmation();
    virtual void askForIDFAConfirmation(const std::function<void(bool)>& onCompletion);

    void addPreInitialization(const SEL_InitializationHandler& callback);
    void addPostInitialization(const SEL_InitializationHandler& callback);

protected:
    
    std::vector<SEL_InitializationHandler> _preInitializationHandlers;
    std::vector<SEL_InitializationHandler> _postInitializationHandlers;
    
    void callInitializationHandlers(std::vector<SEL_InitializationHandler>& handlers);
};

#endif /* OPInitializer_hpp */

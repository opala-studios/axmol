//
//  OPLoadingView_iOS.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#ifndef OPLoadingView_iOS_hpp
#define OPLoadingView_iOS_hpp

#include "OPLoadingView.hpp"

class OPLoadingView_iOS : public OPLoadingView
{
private:
    
    void* impl;
    
protected:
    
    virtual void showView(const std::string& title) override;
    virtual void hideView() override;
    
public:
    
    OPLoadingView_iOS();
    
    virtual void initialize() override;
};

#endif /* OPLoadingView_iOS_hpp */

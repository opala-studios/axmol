//
//  OPLoadingView_Android.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#ifndef OPLoadingView_Android_hpp
#define OPLoadingView_Android_hpp

#include "OPLoadingView.hpp"

class OPLoadingView_Android : public OPLoadingView
{    
protected:
    
    virtual void showView(const std::string& title) override;
    virtual void hideView() override;
    
public:
    
    OPLoadingView_Android();
    
    virtual void initialize() override;
};

#endif /* OPLoadingView_Android_hpp */

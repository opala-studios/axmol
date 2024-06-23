//
//  OPLoadingView.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#ifndef OPLoadingView_hpp
#define OPLoadingView_hpp

#include <iostream>

class OPLoadingView
{
protected:
    
    virtual void showView(const std::string& title) = 0;
    virtual void hideView() = 0;
    
public:

    static OPLoadingView* shared();
    
    static void show(const std::string& title = "");
    static void hide();
    
    virtual void initialize() = 0;
};

#endif /* OPLoadingView_hpp */

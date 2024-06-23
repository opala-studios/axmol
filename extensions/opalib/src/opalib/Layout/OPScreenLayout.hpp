#ifndef OPScreenLayout_hpp
#define OPScreenLayout_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class OPScreenLayout : public cocos2d::ui::Layout
{
public:

    virtual bool init() override;
    
    CREATE_FUNC(OPScreenLayout);
};

#endif /* OPScreenLayout_hpp */
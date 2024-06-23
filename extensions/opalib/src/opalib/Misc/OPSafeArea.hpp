#ifndef OPSafeArea_hpp
#define OPSafeArea_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class OPSafeArea
{
public:
    static float left();
    static float right();
    static float top();
    static float bottom();
    static float width();
    static float height();
    static cocos2d::ui::Margin margin();
};

#endif

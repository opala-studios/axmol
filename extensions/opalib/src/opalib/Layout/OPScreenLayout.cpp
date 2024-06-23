#include "OPScreenLayout.hpp"

USING_NS_CC;

bool OPScreenLayout::init()
{
    if (!ui::Layout::init()) {
        return false;
    }

    setLayoutType(ui::Layout::Type::RELATIVE);
    
    auto winSize = Director::getInstance()->getWinSize();
    setContentSize(winSize);

    return true;
}
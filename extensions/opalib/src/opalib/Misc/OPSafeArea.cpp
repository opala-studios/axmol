#include "OPSafeArea.hpp"

USING_NS_CC;

float OPSafeArea::left()
{
    auto safeArea = cocos2d::Director::getInstance()->getSafeAreaRect();
    return safeArea.getMinX();

}

float OPSafeArea::right()
{
    auto safeArea = cocos2d::Director::getInstance()->getSafeAreaRect();
    return safeArea.getMaxX();
}

float OPSafeArea::top()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static int* safeInsets = JniHelper::callStaticIntArrayMethod("org/cocos2dx/lib/Cocos2dxHelper", "getSafeInsets");
    auto scaleY = cocos2d::Director::getInstance()->getOpenGLView()->getScaleY();

    return safeInsets[3] / scaleY;
#else
    auto winSize = cocos2d::Director::getInstance()->getWinSize();
    auto safeArea = cocos2d::Director::getInstance()->getSafeAreaRect();

    return winSize.height - safeArea.getMaxY();
#endif
}

float OPSafeArea::bottom()
{
    auto safeArea = cocos2d::Director::getInstance()->getSafeAreaRect();
    return safeArea.getMinY();
}

float OPSafeArea::width()
{
    return cocos2d::Director::getInstance()->getSafeAreaRect().size.width;

}

float OPSafeArea::height()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // using this way because cocos return a wrong value when device has softkeyboard
    return cocos2d::Director::getInstance()->getWinSize().height - OPSafeArea::top();
#else
    return cocos2d::Director::getInstance()->getSafeAreaRect().size.height;
#endif
}

cocos2d::ui::Margin OPSafeArea::margin()
{
    auto winSize = cocos2d::Director::getInstance()->getWinSize();
    auto safeArea = cocos2d::Director::getInstance()->getSafeAreaRect();

    return cocos2d::ui::Margin(
        safeArea.getMinX(), winSize.height - safeArea.getMaxY(), winSize.width - safeArea.getMaxX(),
        safeArea.getMinY());
}

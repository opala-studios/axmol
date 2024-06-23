#ifndef OPAdsTestScene_hpp
#define OPAdsTestScene_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class OPAdsTestScene : public cocos2d::Scene
{
public:

    CREATE_FUNC(OPAdsTestScene);

    bool init() override;
    void update(float dt) override;

protected:

    cocos2d::MenuItemFont* _bannerButton;
    cocos2d::MenuItemFont* _interstitialButton;
    cocos2d::MenuItemFont* _rewardedButton;
};

#endif

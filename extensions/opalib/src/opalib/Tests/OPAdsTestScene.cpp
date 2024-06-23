#include "OPAdsTestScene.hpp"
#include "../Ads/OPAds.hpp"

USING_NS_CC;

bool OPAdsTestScene::init()
{
    if (!Scene::init()) {
        return false;
    }

//    OPAds::shared()->initialize(true);

    _bannerButton = MenuItemFont::create("Banner", [] (Ref*)
    {
        if (OPAds::shared()->isBannerWanted()) {
            OPAds::shared()->hideBanner();
        } else {
            OPAds::shared()->showBanner();
        }
    });

    _interstitialButton = MenuItemFont::create("Interstitial", [] (Ref*) {
        OPAds::shared()->showInterstitial([] (const OPAdsResult) {});
    });

    _rewardedButton = MenuItemFont::create("Rewarded", [] (Ref*) {
        OPAds::shared()->showRewarded([](const OPAdsResult) {});
    });

    Vector<MenuItem*> menuItems {_bannerButton, _interstitialButton, _rewardedButton};
    for (auto item : menuItems)
    {
        auto itemFont = (MenuItemFont*)item;
        itemFont->setFontSizeObj(60);
    }

    auto menu = Menu::createWithArray(menuItems);
    menu->alignItemsVerticallyWithPadding(50);
    addChild(menu);

    scheduleUpdate();

    return true;
}

void OPAdsTestScene::update(float dt)
{
    Scene::update(dt);
    
    _bannerButton->setString(OPAds::shared()->isBannerWanted() ? "Hide banner" : "Show banner");
    _interstitialButton->setOpacity(OPAds::shared()->isInterstitialReady() ? 255 : 50);
    _rewardedButton->setOpacity(OPAds::shared()->isRewardedReady() ? 255 : 50);
}

//
// Created by Ricardo on 4/13/2021.
//

#include <Managers/OPAppManager.hpp>
#include <Initializer/OPInitializer.hpp>
#include "OPAppSplashScene.hpp"

using namespace opalib;

void OPAppSplashScene::onEnter() {
    OPBaseScene::onEnter();
    scheduleOnce(
            [this](float) {
                auto appManager = OPAppManager::shared();
                if (appManager->shouldCheckGDPRRequirement()){
                    appManager->getApi()->checkGDPRRequirement([this, appManager](bool required) {
                        appManager->setShouldCheckGDPRRequirement(false);
                        appManager->setGDPRRequired(required);
                        onSplashAnimationEnded();
                    });
                }
                else {
                    onSplashAnimationEnded();
                }
            },
            2.5F,
            "LoadSchedule");
}

void OPAppSplashScene::onSplashAnimationEnded() {

    if (OPInitializer::shared()->needsIDFAConfirmation()){
        OPInitializer::shared()->askForIDFAConfirmation([this](bool allowed){
            advanceToNextScene();
        });
    }
    else {
        advanceToNextScene();
    }
}

void OPAppSplashScene::advanceToNextScene() {
    auto appManager = OPAppManager::shared();
    if (appManager->isGDPRRequired() && !appManager->wasGDPRAlreadyShowed()) {
        auto scene = appManager->getGDPRScene();
        if (scene){
            cocos2d::Director::getInstance()->replaceScene(scene);
            return;
        }
    }
    OPAppManager::shared()->setUserConsent(appManager->userConsent());
    auto scene = appManager->getInitialScene();
    cocos2d::Director::getInstance()->replaceScene(scene);
}

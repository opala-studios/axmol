//
// Created by Ricardo on 4/12/2021.
//

#include <Misc/OPCocosThread.hpp>
#include <Debug/OPDebug.hpp>
#include <Managers/OPAppManager.hpp>
#include "OPSplashScene.hpp"
#include "../TestManager/OPTestManager.hpp"

using namespace opalib;

void OPSplashScene::onEnterTransitionDidFinish() {
    OP_LOG_VERB("BEGIN");
    OPBaseScene::onEnterTransitionDidFinish();
	
	opalib::OPTestManager* manager = opalib::OPTestManager::getInstance();
	manager->initialize([this]() {
		_callNextScene();
	});
	
    OP_LOG_VERB("END");
}

void OPSplashScene::_callNextScene()
{
	auto params = (OPSplashSceneParams*)(_params.get());
	OPCocosThread::invoke("splash_animation_finished", params->waitTime, [params]() {
		auto nextScene = OPAppManager::shared()->getSplashScene();
		if (!nextScene){
			nextScene = OPAppManager::shared()->getInitialScene();
		}
		cocos2d::Director::getInstance()->replaceScene(nextScene);
	});
}

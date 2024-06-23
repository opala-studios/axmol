//
// Created by Talles Borges  on 02/03/20.
//

#include "OPBaseScene.hpp"

#include <boost/make_unique.hpp>
#include <boost/core/demangle.hpp>

#include "../Ads/OPAds.hpp"
#include "../Events/OPEvents.hpp"
#include "../Fairy/OPGRoot.hpp"
#include "../Helpers/OPString.hpp"
#include "../Misc/OPFairySafeArea.hpp"
#include "../Misc/OPSafeArea.hpp"
#include "../Utils/FairyUtils.hpp"
#include "Opalib.h"

USING_NS_CC;
using namespace fairygui;

bool OPBaseScene::init(std::unique_ptr<OPBaseSceneParams> params)
{
    if (!Scene::init()) {
        return false;
    }

    if (params == nullptr) {
        _params = boost::make_unique<OPBaseSceneParams>();
    } else {
        _params = std::move(params);
    }

    //TODO: remake this approach (Talles)
    //caching the packaname to use on destructor of this class
    if(_params->packageName.empty())
    {
        _params->packageName = getPackageName();
    }
    
    _gRoot = OPGRoot::create(this);
    _gRoot->retain();

    if (UIRoot == nullptr) {
        GRoot::setInstance(_gRoot);
    }

    for (const auto& font : getCustomFonts()) {
        UIConfig::registerFont(font.first, font.second);
    }

    for (const auto& packageName : getDependenciesPackagesNames()) {
        fairygui::UIPackage::addPackage(getPackagePath() + packageName);
    }

    for (const auto& customComponent : getCustomComponents()) {
        UIObjectFactory::setPackageItemExtension(customComponent.first, customComponent.second);
    }
    
    if (!getPackageName().empty()) {
        UIPackage::addPackage(getPackagePath() + getPackageName());
        setupScreenBranch();
        _view = UIPackage::createObject(getPackageName(), getViewName())->as<GComponent>();
        _view->setSize(_gRoot->getWidth(), _gRoot->getHeight());
        _gRoot->addChild(_view);
    }

    if (!getBannerAreaViewName().empty()) {
        _bannerArea = _view->getChild(getBannerAreaViewName());
        if (_bannerArea) {
//            _bannerArea->setVisible(false);
            _bannerArea->setHeight(OPAds::shared()->getBannerHeight());
        }
    }

    if (!getSafeAreaViewName().empty()) {
        _safeArea = _view->getChild(getSafeAreaViewName());
        updateSafeAreaBounds();
    }

    for (const auto& item : _params->translations) {
        std::string localizedText;
        if (item.args.empty()) {
            localizedText = OPLocalization::get(item.localizationKey);
        } else {
            localizedText = OPLocalization::get(item.localizationKey, item.args);
        }

        if (item.uppercased){
            localizedText = OPString::uppercased(localizedText);
        }

        if (!item.startText.empty()) {
            localizedText = item.startText + localizedText;
        }

        _view->getChildByPath(item.childPath)->setText(localizedText);
    }

    auto cocosEventListener = EventListenerKeyboard::create();
    cocosEventListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
            onBackPressed();
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(cocosEventListener, this);
	
	_view->addEventListener(UIEventType::TouchBegin, [&](const EventContext* ev) {
		OPEventBus::shared()->notify(OPBaseSceneButtonClickTestEvent{ev->getInput()->getTarget(),
																	 ev,
																	 UIEventType::TouchBegin,
																	 ev->getInput()->getPosition().x,
																	 ev->getInput()->getPosition().y});
	});
	
	_view->addEventListener(UIEventType::TouchEnd, [&](const EventContext* ev) {
		OPEventBus::shared()->notify(OPBaseSceneButtonClickTestEvent{ev->getInput()->getTarget(),
																	 ev,
																	 UIEventType::TouchEnd,
																	 ev->getInput()->getPosition().x,
																	 ev->getInput()->getPosition().y});
	});

    return true;
}

void OPBaseScene::updateSafeAreaBounds()
{
    if (_safeArea) {
        FairyUtils::positionOnSafeArea(_safeArea, requiresBanner() && OPAds::shared()->isBannerWanted());
    }
}

void OPBaseScene::showBannerArea()
{
    if (_bannerArea) {
        _bannerArea->setHeight(OPAds::shared()->getBannerHeight());
        _bannerArea->setWidth(OPAds::shared()->getBannerWidth());
    }
}

void OPBaseScene::hideBannerArea()
{
    if (_bannerArea) {
        _bannerArea->setHeight(0);
        _bannerArea->setWidth(0);
    }
}

void OPBaseScene::pushBackAction(const OPBaseScene::SEL_backPressedAction& action)
{
    _backActionStack.push_back(action);
}

void OPBaseScene::registerBackAction(const OPBaseScene::SEL_backPressedAction& action)
{
    clearBackAction();
    pushBackAction(action);
}

void OPBaseScene::closeScene()
{
    runOnceUntilExit([this]() {
        Director::getInstance()->popScene();
    });
}

void OPBaseScene::runOnceUntilExit(const std::function<void()>& action)
{
    if (!_isWaitingOnExit) {
        _isWaitingOnExit = true;
        action();
    }
}

void OPBaseScene::setIsWaitingOnExit(bool isWaitingOnExit)
{
    _isWaitingOnExit = isWaitingOnExit;
}

void OPBaseScene::onBackPressed()
{
    OP_LOG_VERB("Back Pressed");
    ssize_t c = _backActionStack.size();
    if (c == 1) {
        _backActionStack.at(0)();
        _backActionStack.clear();
        return;
    } else if (c > 1) {
        _backActionStack.at(static_cast<unsigned long>(c - 1))();
        _backActionStack.pop_back();
        return;
    }

    if (!isRoot()) closeScene();
}

void OPBaseScene::onEnter()
{
    Scene::onEnter();
    // quando a tela fica visível, atualiza a area do banner e
    // cria um listener para atualizar também quando o banner
    // mudar de visibilidade
    _bannerVisibilityListener
        = Director::getInstance()->getEventDispatcher()->addCustomEventListener(OPEvents::bannerVisibilityDidChange, [this](EventCustom*) {
              updateBannerArea();
          });

    if (requiresBanner()) {
        OPAds::shared()->showBanner();
    } else {
        OPAds::shared()->hideBanner();
    }
}

void OPBaseScene::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();
	
	OPEventBus::shared()->notify(OPBaseSceneChangeSceneEvent{ boost::core::demangle(typeid(*this).name()) });
}

void OPBaseScene::onExitTransitionDidStart()
{
    Scene::onExitTransitionDidStart();
    // assim que a tela começar a ser fechada, remove o listener
    // para verificar a area do banner, uma vez que não faz sentido
    // atualizar a area do banner se a tela estiver em background
    if (_bannerVisibilityListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_bannerVisibilityListener);
    }
    _bannerVisibilityListener = nullptr;
}

void OPBaseScene::updateBannerArea()
{
    if (requiresBanner() && OPAds::shared()->isBannerWanted()) {
        showBannerArea();
    } else {
        hideBannerArea();
    }

    updateSafeAreaBounds();
}

GComponent* OPBaseScene::getView() const
{
    return _view;
}

GRoot* OPBaseScene::getGRoot() const
{
    return _gRoot;
}

GObject* OPBaseScene::getSafeArea() const
{
    return _safeArea;
}

GObject* OPBaseScene::getBannerArea() const
{
    return _bannerArea;
}

void OPBaseScene::onExit()
{
    Scene::onExit();
    _isWaitingOnExit = false;
}

void OPBaseScene::setupScreenBranch()
{
    if(Opalib::shared()->getXSize() <= 1)return;
    
    _screenBranch = "2x";//std::to_string(Opalib::shared()->getXSize()) + "x";
    UIPackage::setBranch(_screenBranch);
}

OPBaseScene::~OPBaseScene()
{
    OP_LOG("BEGIN");
    CC_SAFE_RELEASE_NULL(_gRoot);
    fairygui::UIPackage::removePackage(getPackagePath() + getPackageName());
    OP_LOG("END");
}

void OPBaseScene::bindClickListener(
        const std::string &path,
        const EventCallback &callback,
        const EventTag &tag
) const {
    fairygui::GObject *obj;
    if (findChild<fairygui::GObject>(path, &obj)){
        obj->addClickListener(callback, tag);
    }
}

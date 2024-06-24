//
//  Opalib.cpp
//  Opalib
//
//  Created by Adilson Tavares on 16/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

//#include "./Scene/OPSplashScene.hpp"
#include "Opalib.h"
//#include "./Initializer/OPInitializer.hpp"

USING_NS_CC;

Opalib* Opalib::shared()
{
    static Opalib* instance = new Opalib();
    return instance;
}

Opalib::Opalib()
{
    initialized = false;
    
#if COCOS2D_DEBUG
    setDebugMode(true);
#else
    setDebugMode(false);
#endif
}

void Opalib::setDebugMode(bool debug)
{
    debugMode = debug;
    Director::getInstance()->setDisplayStats(debug);
}

void Opalib::initialize()
{
    if (initialized) { return; }
    initialized = true;

    OPDebug::log("Opalib", "Package name = " + getPackageName());
    OPDebug::log("Opalib", "Version = " + getAppVersion());

//    rapidjson::Document document;
//    OPDebug::ensure(OPJson::parseFile("config/app.json", document), "App Config at config/app.json coult not be loaded.");
//
//    appConfig = OPAppConfig::create(document);
//    OPDebug::ensure(appConfig, "App Config could not be loaded.");
//
//    appConfig->retain();
//
//    auto temp = OPDirectory::getTemp();
//
//    FileUtils::getInstance()->removeDirectory(temp);
//    FileUtils::getInstance()->createDirectory(temp);
//
//    if (appConfig->getUsesFairyGUI())
//    {
//        if (appConfig->getUsesCocos()) {
//            setupViewForHybrid();
//        }
//        else {
//            setupViewForFairyGUI();
//        }
//    }
//    else {
//        setupViewForCocos();
//    }
    
//    setupDeviceTypeAndSize();

    setupViewForFairyGUI();
//    Director::getInstance()->setDisplayStats(debugMode);

//    OPInitializer::shared()->initialize();
}

void Opalib::setupViewForFairyGUI()
{
//    auto director = Director::getInstance();
//    auto glview = director->getOpenGLView();
//    auto designResolution = appConfig->getDesignResolution();
//
//    if (!glview)
//    {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//        auto designScale = appConfig->getContentScaleFactor();
//        glview = GLViewImpl::createWithRect(appConfig->getTitle(), cocos2d::Rect(0, 0, designResolution.width / designScale, designResolution.height / designScale));
//#else
//        glview = GLViewImpl::create(appConfig->getTitle());
//#endif
//        director->setOpenGLView(glview);
//    }
//
//    director->setAnimationInterval(1.0f / 60);
//
//    auto frameSize = glview->getFrameSize();
//    //auto screenAspect = frameSize.height / frameSize.width;
//    auto aspectRatio = frameSize.height / frameSize.width;
//    if (aspectRatio > 1) aspectRatio = 1 / aspectRatio;
//    OPDebug::log("TABLET SCREEN", "Width: " + std::to_string(frameSize.width) + " Height: " + std::to_string(frameSize.height) + " Aspect Ratio: " + std::to_string(aspectRatio));
//
//    if (aspectRatio <= 0.563) {//screenAspect >= 1.5f || frameSize.height < designResolution.height) {
////        glview->setDesignResolutionSize(designResolution.width, designResolution.height, appConfig->getResolutionPolicy());
//        _isTabletResolution = false;
//
//    } else { // Tablets
//        _isTabletResolution = true;
////        glview->setDesignResolutionSize(frameSize.width, frameSize.height, appConfig->getResolutionPolicy());
//    }
//
//    //TODO:: Usar branchs do fairy pra solucionar isso #############
//    ResolutionPolicy policy;
//    if(_isTabletResolution)
//        policy = ResolutionPolicy::FIXED_WIDTH;
//    else
//        policy =ResolutionPolicy::FIXED_HEIGHT;
//
//    glview->setDesignResolutionSize(designResolution.width, designResolution.height, policy);
}
//
//
//void Opalib::setupViewForCocos()
//{
//    auto director = Director::getInstance();
//    auto glview = director->getOpenGLView();
//    auto designResolution = appConfig->getDesignResolution();
//
//    if (!glview)
//    {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//        glview = GLViewImpl::createWithRect(appConfig->getTitle(), cocos2d::Rect(0, 0, designResolution.width, designResolution.height));
//#else
//        glview = GLViewImpl::create(appConfig->getTitle());
//#endif
//        director->setOpenGLView(glview);
//    }
//
//    director->setAnimationInterval(1.0f / 60);
//
//    // Set the design resolution
//    auto frameSize = glview->getFrameSize();
//    auto aspect = frameSize.width / frameSize.height;
//    auto width = aspect * designResolution.height;
//
//    //TODO:: Usar branchs do fairy pra solucionar isso #############
////    glview->setDesignResolutionSize(width, designResolution.height, appConfig->getResolutionPolicy());
//
//    ResolutionPolicy policy;
//    if(_isTabletResolution)
//        policy = ResolutionPolicy::FIXED_WIDTH;
//    else
//        policy =ResolutionPolicy::FIXED_HEIGHT;
//
//    glview->setDesignResolutionSize(width, designResolution.height, policy);
//    //##############################################################
//
//    std::vector<std::string> searchPaths;
//
//    auto path = OPPath::join(appConfig->getImagesPath(), "hd");
//    searchPaths.push_back(path);
//    director->setContentScaleFactor(1);
//
//    FileUtils::getInstance()->setSearchPaths(searchPaths);
//}
//
//void Opalib::setupViewForHybrid()
//{
//    setupViewForCocos();
//    setupViewForFairyGUI();
//}

void Opalib::start(cocos2d::Scene* scene)
{
    if (!initialized)
    {
        OPDebug::error("SetupViewForFairyGUI","You must initialize Opalib before starting it.");
        return;
    }
    
    Director::getInstance()->runWithScene(scene);
}

void Opalib::applicationDidEnterBackground()
{
    OP_LOG("BEGIN");
//    OPEventBus::shared()->notify(OPEvents::ApplicationDidEnterBackground());
//    OPApplication::shared()->getAudioEngine()->pause();
    Director::getInstance()->stopAnimation();
    OP_LOG("END");
}

void Opalib::applicationWillEnterForeground()
{
    OP_LOG("BEGIN");
//    OPEventBus::shared()->notify(OPEvents::ApplicationWillEnterForeground());
//    OPApplication::shared()->getAudioEngine()->resume();
    Director::getInstance()->startAnimation();
    OP_LOG("END");
}

void Opalib::initGLContextAttrs()
{
    // set OpenGL context attributes: red, green, blue, alpha, depth, stencil, multisamplesCount
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

int Opalib::getXSize()
{
    return _xSize;
}

bool Opalib::isTablet()
{
    return _isTabletResolution;
}

void Opalib::setupDeviceTypeAndSize()
{
    auto director =  Director::getInstance();
    int dpi = Device::getDPI();
    
    float xInches = director->getWinSize().width / dpi;
    float yInches = director->getWinSize().height / dpi;
    float diagonalInches = sqrtf(xInches * xInches + yInches * yInches);
    diagonalInches = roundf(diagonalInches * 100.0f) / 100.0f;
    
    bool isTablet = (diagonalInches >= 7.0f);
    
    int multiplyer = round(dpi/160.0f);
    
    if(multiplyer > 3)
        multiplyer = 3;
    
    _isTabletResolution = isTablet;
    
    _xSize = multiplyer;
}

void Opalib::end()
{

}

Opalib::~Opalib()
{
    OP_LOG("BEGIN");
//    CC_SAFE_RELEASE_NULL(appConfig);
    OP_LOG("END");
}

bool Opalib::isTabletResolution()
{
    return _isTabletResolution;
}



//
//  OPTestManager.cpp
//
//  Created by Allison Lindner on 07/06/21.
//

#include "OPTestManager.hpp"
#include "OPTestEvent.hpp"
#include "../API/OPTestLabAPI.hpp"
#include "../Scene/OPBaseScene.hpp"
#include "../Utils/OPFairyUtils.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Opalib.h"

static opalib::OPTestManager* s_testManager = nullptr;

using namespace fairygui;

opalib::OPTestManager* opalib::OPTestManager::getInstance()
{
	if(!s_testManager)
	{
		s_testManager = new opalib::OPTestManager();
		
		auto* glView = cocos2d::Director::getInstance()->getOpenGLView();
		
		glView->setTouchesBeginCallback(CC_CALLBACK_3(OPTestManager::handleTouchesBegin, s_testManager));
		glView->setTouchesMoveCallback(CC_CALLBACK_3(OPTestManager::handleTouchesMove, s_testManager));
		glView->setTouchesEndCallback(CC_CALLBACK_3(OPTestManager::handleTouchesEnd, s_testManager));
	}
	
	return s_testManager;
}

void opalib::OPTestManager::initialize(std::function<void()> handler)
{
    
    _notificationListenerId = OPNotificationManager::shared()->addNotificationListener([this](OPNotificationManager::Notification& notification) -> bool {
        
        auto& payload = notification.payload;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if (!payload.HasMember("action")){
			return false;
		}
		std::string action = payload["action"].GetString();

		if (action != "com.google.intent.action.TEST_LOOP"){
			return false;
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if (!payload.HasMember("scheme")){
            return false;
        }
        std::string scheme = payload["scheme"].GetString();

        if (scheme != "firebase-game-loop"){
            return false;
        }
#endif

        int scenario;
        if (!payload.HasMember("scenario")){
            scenario = 0;
        }
        else {
			scenario = std::stoi(payload["scenario"].GetString());
        }
        
        setScenarioFromFirebase(scenario);
        //02435CB7-9050-4552-923E-E8A7E8181CF9
        return true;
    });
    
    OPNotificationManager::shared()->dispatchNotification();

	if(!_testLabSetup)
	{
		_currentMode = opalib::OPTestMode_enum.at(Opalib::shared()->getAppConfig()->testConfig.action);
		_currentScenerio = Opalib::shared()->getAppConfig()->testConfig.scenario;
	}
	
	_currentProject = Opalib::shared()->getAppConfig()->testConfig.projectName;
	
	switch (_currentMode) {
		case opalib::OPTestMode::RECORDING:
			setupRecording();
			handler();
			break;
		case opalib::OPTestMode::RUNNING:
			run(handler);
			break;
		case opalib::OPTestMode::NONE:
			handler();
			break;
	}
    
}
/**
 The setScenarioFromFirebase method should be called before OPTestManager::initialize !!!!!
 */
void opalib::OPTestManager::setScenarioFromFirebase(int scenario)
{
	_currentMode = RUNNING;
	_currentScenerio = scenario;
	_testLabSetup = true;
	
	cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(opalib::OPFinishTestEvent::kEventName, [this](cocos2d::EventCustom* ev)
    {
        _debugDrawNode->clear();
        _debugDrawNode = nullptr;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //Android Here
        cocos2d::JniHelper::callStaticVoidMethod("com/opalastudios/opalib/activity/OpalibActivity", "finishTestLab");
        cocos2d::Director::getInstance()->end();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		cocos2d::Application::getInstance()->openURL("firebase-game-loop-complete://");
        cocos2d::Director::getInstance()->end();
        exit(0);
#endif
	});

}

void opalib::OPTestManager::setupRecording()
{
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	dispatcher->addCustomEventListener(GListTouchBeginEvent::kEventName, [this](cocos2d::EventCustom* ev) {
		GListTouchBeginEvent* data = (GListTouchBeginEvent*) ev->getUserData();
		
		_currentListComponentPath = OPFairyUtils::getFullPath(data->component);
		
		_touchToProcess[_currentListComponentPath].componentPath = _currentListComponentPath;
		_touchToProcess[_currentListComponentPath].listItemIndex = data->itemIndex;
		_touchToProcess[_currentListComponentPath].listTouch = true;
		_touchToProcess[_currentListComponentPath].time = _currentTimelapse;
		_currentTimelapse = 0.0f;
	});
	
	_listener.listen<OPBaseSceneButtonClickTestEvent>([&](const OPBaseSceneButtonClickTestEvent& ev) {
		std::string componentPath = OPFairyUtils::getFullPath(ev.component);
		std::string listItemComponentPath;
		
		auto pos = cocos2d::Vec2(ev.posX, ev.posY);
		float posX = pos.x * OPFairyUtils::getScreenFactor().x;
		float posY = pos.y * OPFairyUtils::getScreenFactor().y;
		
		if(!_currentListComponentPath.empty())
		{
			_touchToProcess[_currentListComponentPath].componentPath = _currentListComponentPath;
			if(componentPath.size() > _currentListComponentPath.size())
			{
				listItemComponentPath.replace(listItemComponentPath.begin(),
											  listItemComponentPath.end(),
											  componentPath.begin() + (_currentListComponentPath.size() + 1),
											  componentPath.end());
				componentPath = _currentListComponentPath;
				_touchToProcess[componentPath].listItemComponentPath = listItemComponentPath;
				
				OPDebug::log("OPTestManager", "List Item Component Clicked Path: " + listItemComponentPath);
			}
		}
		
		if(ev.eventType == fairygui::UIEventType::TouchBegin) {
			OPDebug::log("OPTestManager", "Component Clicked Path: " + componentPath + " BEGIN - X: " + std::to_string(posX) + " Y: " + std::to_string(posY));
			
			auto it = _touchToProcess.find(componentPath);
			if(it == _touchToProcess.end())
			{
				_touchToProcess[componentPath].componentPath = componentPath;
				_touchToProcess[componentPath].time = _currentTimelapse;
				_currentTimelapse = 0.0f;
			}
			
			_touchToProcess[componentPath].deltaPosX = posX;
			_touchToProcess[componentPath].deltaPosY = posY;
		} else if(ev.eventType == fairygui::UIEventType::TouchEnd) {
			OPDebug::log("OPTestManager", "Component Clicked Path: " + componentPath + " END   - X: " + std::to_string(posX) + " Y: " + std::to_string(posY));
			
			_touchToProcess[componentPath].deltaPosX = posX - _touchToProcess[componentPath].deltaPosX;
			_touchToProcess[componentPath].deltaPosY = posY - _touchToProcess[componentPath].deltaPosY;
			
			if((_touchToProcess[componentPath].deltaPosX > 1.0f || _touchToProcess[componentPath].deltaPosX < -1.0f) ||
			   (_touchToProcess[componentPath].deltaPosY > 1.0f || _touchToProcess[componentPath].deltaPosY < -1.0f)) {
				_touchToProcess[componentPath].eventType = TOUCH_MOVE;
			} else {
				_touchToProcess[componentPath].eventType = TOUCH;
			}
			
			createTestEvent(_touchToProcess[componentPath]);
			
			_touchToProcess.erase(componentPath);
			_currentListComponentPath = "";
		}
	});
	
	s_testManager->_listener.listen<OPBaseSceneChangeSceneEvent>([&](const OPBaseSceneChangeSceneEvent& ev) {
		OPDebug::log("OPTestManager", "Change to Scene: " + ev.sceneName);
		auto touchInfo = TouchInfo();
		
		touchInfo.eventType = WAIT_TAG;
		touchInfo.tag = ev.sceneName;
		
		createTestEvent(touchInfo);
		
		_currentTimelapse = 0.0f;
	});
	
	_listener.listen<OPEvents::ApplicationDidEnterBackground>([&](const OPEvents::ApplicationDidEnterBackground& ev) {
		if(!_saved)
			save();
	});
	
	cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void opalib::OPTestManager::setupDebug() {
	OPBaseScene* runningScene = (OPBaseScene*) cocos2d::Director::getInstance()->getRunningScene();
	
	_debugDrawNode = (cocos2d::DrawNode*) runningScene->getView()->displayObject()->getChildByName("debugDrawNode");
	
	if(!_debugDrawNode)
	{
		_debugDrawNode = cocos2d::DrawNode::create();
		_debugDrawNode->setName("debugDrawNode");
		_debugDrawNode->setContentSize(runningScene->getView()->getSize());
		runningScene->getView()->displayObject()->addChild(_debugDrawNode);

		_debugDrawNode->setPosition(0.0f, 0.0f);
	}
	
	_debugDrawNode->clear();
}

void opalib::OPTestManager::createTestEvent(TouchInfo touchInfo)
{
	opalib::OPTestEvent event = opalib::OPTestEvent(touchInfo);
	
	_test.addEvent(event);
}

void opalib::OPTestManager::run(std::function<void()> handler)
{
	OPTestLabAPI(1).loadAppTest(_currentProject, _currentScenerio, [this, handler](OPTest test) {
		_test = test;
		
		cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

		s_testManager->_listener.listen<OPBaseSceneChangeSceneEvent>([this](const OPBaseSceneChangeSceneEvent& ev) {
			OPDebug::log("OPTestManager", "Change to Scene: " + ev.sceneName);
			_debugDrawNode = nullptr;
		});

		_listener.listen<ScheduleNextEvent>([this](const ScheduleNextEvent& ev) {
			if(_debugDrawNode)
				_debugDrawNode->clear();
			
			_test.scheduleNextEvent();
		});

		_test.scheduleNextEvent();
		
		handler();
	});
}

void opalib::OPTestManager::save()
{
	auto document = OPJson::document();
	
	_test.toJson(&document);
	
	OPTestLabAPI(1).saveAppTest(&document, _currentProject, _currentScenerio, [this](bool success) {
		if(success)
			OPDebug::log("OPTestManager", "Test Saved");
		
		_saved = success;
	});
}

void opalib::OPTestManager::update(float dt)
{
	if(_currentMode == OPTestMode::RECORDING) {
		_currentTimelapse += dt;
		
		for(auto& touch: _touchToProcess)
		{
			touch.second.timelapse += dt;
		}
	}

	if(_currentMode == OPTestMode::RUNNING)
		_test.update(dt);
}

void opalib::OPTestManager::handleTouchesBegin(int touchId, float x, float y)
{
//	OPDebug::log("OPTestHandler", "handleTouchesBegin - x: " + std::to_string(x) + " y: " + std::to_string(y));
	if(_currentMode == RUNNING)
		_drawDot(cocos2d::Vec2(x, y));
}

void opalib::OPTestManager::handleTouchesMove(int touchId, float x, float y)
{
//	OPDebug::log("OPTestHandler", "handleTouchesMove - x: " + std::to_string(x) + " y: " + std::to_string(y));
	if(_currentMode == RUNNING)
		_drawDot(cocos2d::Vec2(x, y));
}

void opalib::OPTestManager::handleTouchesEnd(int touchId, float x, float y)
{
//	OPDebug::log("OPTestHandler", "handleTouchesEnd - x: " + std::to_string(x) + " y: " + std::to_string(y));
	if(_currentMode == RUNNING)
		_drawDot(cocos2d::Vec2(x, y));
}

void opalib::OPTestManager::_drawDot(cocos2d::Vec2 position)
{
	cocos2d::Vec2 pos = position;
    
    float height = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().height;
    
	pos.y = height - pos.y;
    
    pos.x /= OPFairyUtils::getScreenFactor().x;
    pos.y /= OPFairyUtils::getScreenFactor().y;

	
	if(!_debugDrawNode)
		setupDebug();
	
	if(_debugDrawNode) {
		_debugDrawNode->clear();
		_debugDrawNode->drawSolidCircle(pos, 20.0f, MATH_DEG_TO_RAD(360), 20, cocos2d::Color4F::MAGENTA);
	}
}

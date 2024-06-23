//
//  OPTestEvent.cpp
//
//  Created by Allison Lindner on 07/06/21.
//

#include "OPTestEvent.hpp"

#include "../Scene/OPBaseScene.hpp"

#include <stdio.h>

#include "OPTestManager.hpp"
#include "OPTest.hpp"
#include "../Utils/OPFairyUtils.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Misc/OPCocosThread.hpp"

using namespace opalib;

opalib::OPTestEvent::OPTestEvent()
{
	_eventID = 0;
}

opalib::OPTestEvent::OPTestEvent(TouchInfo touchInfo)
{
	_eventID = 0;
	_time = touchInfo.time;
	_duration = touchInfo.timelapse;
	_componentPath = touchInfo.componentPath;
	_listItemComponentPath = touchInfo.listItemComponentPath;
	_deltaX = touchInfo.deltaPosX;
	_deltaY = touchInfo.deltaPosY;
	_listItemIndex = touchInfo.listItemIndex;
	_listTouch = touchInfo.listTouch;
	_eventType = touchInfo.eventType;
	_tag = touchInfo.tag;
}

void opalib::OPTestEvent::setupEvent()
{
	OPBaseScene* runningScene = (OPBaseScene*) cocos2d::Director::getInstance()->getRunningScene();
	fairygui::GObject* component = dynamic_cast<fairygui::GObject*>(runningScene->getView()->getChildByPath(_componentPath));
	
	fairygui::GList* glist = nullptr;
	if(_listTouch)
	{
		glist = dynamic_cast<fairygui::GList*>(component);
		if(glist)
		{
			int childIndex = glist->itemIndexToChildIndex(_listItemIndex);
			component = glist->getChildAt(childIndex);
			
			if(!_listItemComponentPath.empty())
			{
				auto* comp = component->as<fairygui::GComponent>();
				if(comp)
					component = comp->getChildByPath(_listItemComponentPath);
			}
		}
	}
	
	if(_componentPath != "view" && component)
	{
		cocos2d::Vec2 position = OPFairyUtils::getScreenPosition(component);
		
		if(_listTouch)
		{
			if(glist)
			{
                position.x = position.x - glist->getScrollPane()->getScrollingPosX();
                position.y = position.y - glist->getScrollPane()->getScrollingPosY();
			}
		}
		
		_startX =(position.x + (component->getWidth() * 0.5f)) * OPFairyUtils::getScreenFactor().x;
		_startY =(position.y + (component->getHeight() * 0.5f)) * OPFairyUtils::getScreenFactor().y;
	}
	
	if(_eventType == TOUCH_MOVE)
	{
		_easyInOutCurveX = OPCurve<float>::easeInOut(_startX, _startX + _deltaX);
		_easyInOutCurveY = OPCurve<float>::easeInOut(_startY, _startY + _deltaY);
	}
}

void opalib::OPTestEvent::setId(int id)
{
	_eventID = id;
}

void opalib::OPTestEvent::schedule(OPTest& test, float timelapseDiff)
{
	float diff = _time - timelapseDiff;
	if(diff < 0.0f)
		diff = _time;
	
	if(_eventType == OPTestEventType::TOUCH || _eventType == OPTestEventType::TOUCH_MOVE)
	{
		OPDebug::log("OPTestManager", std::to_string(_eventID) + " - Scheduled Timelapse: " + std::to_string(diff));
		
		OPCocosThread::invoke("OPTestEvent_" + std::to_string(_eventID) + "_begin", diff, [this, &test]() {
			OPEventBus::shared()->notify(opalib::ScheduleNextEvent{});
			setupEvent();
			
			OPDebug::log("OPTestManager", "ScreenSize - W: " +
						 std::to_string(cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().width) + " H: " +
						 std::to_string(cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().height));

			OPDebug::log("OPTestManager", "SceneViewSize - W: " +
						 std::to_string(cocos2d::Director::getInstance()->getRunningScene()->getContentSize().width) + " H: " +
						 std::to_string(cocos2d::Director::getInstance()->getRunningScene()->getContentSize().height));
			
			
			executeTouch([this](int num, intptr_t* ids, float* xs, float* ys)
			{
				xs[0] = _startX;
				ys[0] = _startY;
				
				cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesBegin(num, ids, xs, ys);
			}, BEGIN);
			
			if(_eventType == OPTestEventType::TOUCH_MOVE)
			{
				_accumulatedTime = 0.0f;
				test.addEventToUpdate(_eventID);
			}
			
			OPCocosThread::invoke("OPTestEvent_" + std::to_string(_eventID) + "_end", _duration, [this, &test]()
			{
				if(_eventType == OPTestEventType::TOUCH_MOVE)
					test.removeEventToUpdate(_eventID);
				
				executeTouch([this](int num, intptr_t* ids, float* xs, float* ys)
				{
					xs[0] = _startX + _deltaX;
					ys[0] = _startY + _deltaY;
					
					cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesEnd(num, ids, xs, ys);
				}, END);
			});
		});
	} else if(_eventType == OPTestEventType::WAIT_TAG)
	{
		_listener.listen<OPBaseSceneChangeSceneEvent>([this](const OPBaseSceneChangeSceneEvent& ev)
		{
			if(_tag == ev.sceneName)
			{
				_listener.unlisten<OPBaseSceneChangeSceneEvent>();
				OPEventBus::shared()->notify(opalib::ScheduleNextEvent{});
			}
		});
	}
}

void opalib::OPTestEvent::executeTouch(SEL_touchHandler touchHandler, OPTestEventTouchType eventType)
{
	intptr_t ids[1] = {_eventID};
	
	float Xs[1] = {0.0f};
	float Ys[1] = {0.0f};
	
	std::string description;
	if(eventType == BEGIN)
		description = "BEGIN";
	else if(eventType == END)
		description = "END";
	
	touchHandler(1, ids, Xs, Ys);
	
	OPDebug::log("OPTestManager",
				 "Touch Component Path: " + _componentPath + "." + _listItemComponentPath + " at X: " + std::to_string(Xs[0]) + " Y: " + std::to_string(Ys[0]) + " " + description);
}

void opalib::OPTestEvent::update(float deltaTime)
{
	if(_eventType == TOUCH_MOVE)
	{
		_accumulatedTime += deltaTime;
		
		intptr_t ids[1] = {_eventID};
		
		float lerpX[1] = { _easyInOutCurveX.evaluate(_accumulatedTime/_duration) };
		float lerpY[1] = { _easyInOutCurveY.evaluate(_accumulatedTime/_duration) };
		
		cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesMove(1, ids, lerpX, lerpY);
	}
}

void opalib::OPTestEvent::parse(const rapidjson::Value& value)
{
	std::string type(value["type"].GetString());
	_eventType = OPTestEventType_enum.at(type);
	
	if(_eventType == OPTestEventType::WAIT_TAG)
	{
		_tag = value["tag"].GetString();
	} else if(_eventType == OPTestEventType::TOUCH || _eventType == OPTestEventType::TOUCH_MOVE)
	{
		_time = value["time"].GetFloat();
		_duration = value["duration"].GetFloat();
		_componentPath = value["component_path"].GetString();
		_listItemComponentPath = value["list_item_component_path"].GetString();
		_listItemIndex = value["list_item_index"].GetInt();
		_listTouch = value["list_touch"].GetBool();
		
		if(_eventType == OPTestEventType::TOUCH_MOVE)
		{
			_deltaX = value["deltaX"].GetFloat();
			_deltaY = value["deltaY"].GetFloat();
		}
	}
}

rapidjson::Value opalib::OPTestEvent::toJson(rapidjson::Document *document) const
{
	rapidjson::Value value = OPJson::object();
	
	OPJson::set(*document, value, "time", _time);
	OPJson::set(*document, value, "duration", _duration);
	OPJson::set(*document, value, "component_path", _componentPath);
	OPJson::set(*document, value, "list_item_component_path", _listItemComponentPath);
	OPJson::set(*document, value, "tag", _tag);
	OPJson::set(*document, value, "type", OPTestEventType_str[_eventType]);
	OPJson::set(*document, value, "deltaX", _deltaX);
	OPJson::set(*document, value, "deltaY", _deltaY);
	OPJson::set(*document, value, "list_item_index", _listItemIndex);
	OPJson::set(*document, value, "list_touch", _listTouch);
	
	return value;
}


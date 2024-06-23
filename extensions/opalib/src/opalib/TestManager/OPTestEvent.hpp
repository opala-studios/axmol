//
//  OPTestEvent.hpp
//
//  Created by Allison Lindner on 07/06/21.
//

#ifndef OPTestEvent_hpp
#define OPTestEvent_hpp

#include "../Json/OPJson.hpp"
#include "../Math/OPCurve.hpp"
#include "../Misc/OPEventBus.hpp"

namespace opalib
{

class OPTest;

enum OPTestEventType
{
	TOUCH,
	TOUCH_MOVE,
	WAIT_TAG
};

enum OPTestEventTouchType
{
	BEGIN,
	END
};

struct TouchInfo
{
	std::string componentPath;
	std::string listItemComponentPath;
	float time = 0.0f;
	float timelapse = 0.0f;
	float deltaPosX = 0.0f;
	float deltaPosY = 0.0f;
	int listItemIndex = 0;
	bool listTouch = false;
	std::string tag;
	OPTestEventType eventType = TOUCH;
};

static const char *OPTestEventType_str[] = { "TOUCH", "TOUCH_MOVE", "WAIT_TAG" };
static const std::unordered_map<std::string, OPTestEventType> OPTestEventType_enum = {
	{"TOUCH", OPTestEventType::TOUCH},
	{"TOUCH_MOVE", OPTestEventType::TOUCH_MOVE},
	{"WAIT_TAG", OPTestEventType::WAIT_TAG}
};

class OPTestEvent
{
	
public:
	typedef std::function<void(int, intptr_t*, float*, float*)> SEL_touchHandler;
	
	OPTestEvent();
	OPTestEvent(TouchInfo touchInfo);
	
	void setId(int id);
	float getTime() { return _time; }
	float getDuration() { return _duration; }
	OPTestEventType getEventType() { return _eventType; }
	
	void schedule(OPTest& test, float timelapseDiff = 0.0f);
	void update(float deltaTime);
	
	void parse(const rapidjson::Value& value);
	rapidjson::Value toJson(rapidjson::Document* document) const;
	
private:
	Dexode::TokenHolder<Dexode::EventBus> _listener{OPEventBus::shared()};
	
	int _eventID = 0;
	
	float _time = 0.0f;
	float _duration = 0.0f;
	float _deltaX = 0.0f;
	float _deltaY = 0.0f;
	
	int _listItemIndex = 0;
	bool _listTouch = false;
	
	std::string _componentPath;
	std::string _listItemComponentPath;
	std::string _tag;
	OPTestEventType _eventType;
	
	float _startX = 0.0f;
	float _startY = 0.0f;
	
	float _accumulatedTime = 0.0f;
	
	OPCurve<float> _easyInOutCurveX;
	OPCurve<float> _easyInOutCurveY;
	
	void setupEvent();
	void executeTouch(SEL_touchHandler touchHandler, OPTestEventTouchType eventType);
	
};

}

#endif /* OPTestEvent_hpp */

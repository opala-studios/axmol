//
//  OPTest.cpp
//
//  Created by Allison Lindner on 07/06/21.
//

#include "OPTest.hpp"
#include "OPTestEvent.hpp"
#include "../Opalib.h"
#include "../Debug/OPDebug.hpp"

const char* opalib::OPFinishTestEvent::kEventName = "OPFinishTestEvent";

void opalib::OPTest::addEvent(OPTestEvent event)
{
	event.setId((int) _events.size());
	_events.push_back(std::move(event));
}

void opalib::OPTest::addEventToUpdate(int index)
{
	_indexEventsToUpdate.insert(index);
}

void opalib::OPTest::removeEventToUpdate(int index)
{
	_indexEventsToUpdate.erase(index);
}

void opalib::OPTest::update(float deltaTime)
{
	for(int indexEventToUpdate: _indexEventsToUpdate)
	{
		_events[indexEventToUpdate].update(deltaTime);
	}
	
	_timelapse += deltaTime;
}

void opalib::OPTest::scheduleNextEvent()
{
	if(_currentEventIndex < _events.size()) {
		if(_eventsTimeAccumulated == 0.0f)
			_timelapse = 0.0f;
		
		float diff = _timelapse - _eventsTimeAccumulated;
		_eventsTimeAccumulated += _events[_currentEventIndex].getTime();
		
		if(_events[_currentEventIndex].getEventType() == WAIT_TAG)
		{
			diff = 0.0f;
			_eventsTimeAccumulated = 0.0f;
		}
		
		OPDebug::log("OPTestManager", std::to_string(_currentEventIndex) + " - To Schedule Timelapse Diff: " + std::to_string(diff));
		
		_events[_currentEventIndex].schedule(*this, diff);
		_currentEventIndex++;
	} else {
		float time = _events[_events.size() - 1].getTime() + _events[_events.size() - 1].getDuration() + 2.0f;
		
		OPCocosThread::invoke("OPTest_finishTest", time, [this]()
		{
			auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
			dispatcher->dispatchCustomEvent(OPFinishTestEvent::kEventName, nullptr);
		});
	}
}

void opalib::OPTest::toJson(rapidjson::Document *document)
{
	rapidjson::Value eventsArray = OPJson::array();
	for(const auto& event: _events)
	{
		rapidjson::Value eventValue = event.toJson(document);
		OPJson::append(*document, eventsArray, eventValue);
	}
	
	OPJson::set(*document, "events", eventsArray);
}

void opalib::OPTest::parse(const rapidjson::Document& document)
{
	if(OPJson::hasArray(document, "events") && document["events"].GetArray().Size() > 0)
	{
		for(const auto& event : document["events"].GetArray())
		{
			opalib::OPTestEvent ev;
			ev.parse(event);
			addEvent(ev);
		}
	}
}

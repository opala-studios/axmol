//
//  OPTest.hpp
//
//  Created by Allison Lindner on 07/06/21.
//

#ifndef OPTest_hpp
#define OPTest_hpp

#include <stdio.h>
#include "OPTestEvent.hpp"

// Representa um único cenário de teste
namespace opalib
{

class OPTestEvent;

struct WaitEvent
{
	std::string tag;
};

struct OPFinishTestEvent
{
	static const char* kEventName;
};

class OPTest
{
	
public:
	int scenarioId;
	
	int getCurrentEventIndex() { return _currentEventIndex; };
	std::vector<OPTestEvent>& getEvents() { return _events; };
	
	void addEvent(OPTestEvent event);
	void addEventToUpdate(int index);
	void removeEventToUpdate(int index);
	void scheduleNextEvent();
	
	void update(float deltaTime);
	
	void toJson(rapidjson::Document* document);
	void parse(const rapidjson::Document& document);
	
private:
	std::vector<OPTestEvent> _events = {};
	std::set<int> _indexEventsToUpdate = {};
	
	int _currentEventIndex = 0;
	float _timelapse = 0.0f;
	float _eventsTimeAccumulated = 0.0f;
	
};

}

#endif /* OPTest_hpp */

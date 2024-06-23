//
//  OPTestManager.hpp
//
//  Created by Allison Lindner on 07/06/21.
//

#ifndef OPTestManager_hpp
#define OPTestManager_hpp

#include <stdio.h>
#include "../Misc/OPEventBus.hpp"
#include "OPTest.hpp"
#include "OPTestEvent.hpp"

namespace opalib
{

enum OPTestMode
{
	NONE,
	RECORDING,
	RUNNING
};

static const char *OPTestMode_str[] = { "NONE", "REC", "RUN" };
static const std::unordered_map<std::string, OPTestMode> OPTestMode_enum = {
	{"NONE", OPTestMode::NONE},
	{"REC", OPTestMode::RECORDING},
	{"RUN", OPTestMode::RUNNING}
};

struct ScheduleNextEvent {};

class OPTestManager
{
	
public:
	static OPTestManager* getInstance();
	
	void initialize(std::function<void()> handler);
	void setScenarioFromFirebase(int scenario);
	
	void setupRecording();
	void setupDebug();
	
	void createTestEvent(TouchInfo touchInfo);
	
	void run(std::function<void()> handler);
	void save();
	
	void update(float dt);
	
	void handleTouchesBegin(int touchId, float x, float y);
	void handleTouchesMove(int touchId, float x, float y);
	void handleTouchesEnd(int touchId, float x, float y);
	
private:
	Dexode::TokenHolder<Dexode::EventBus> _listener{OPEventBus::shared()};
	
	OPTest _test;
	std::map<std::string, TouchInfo> _touchToProcess;
	
	std::string _currentListComponentPath = "";
	
	float _currentTimelapse = 0.0f;
	
	OPTestMode _currentMode = OPTestMode::NONE;

	int _currentScenerio = 0;
	std::string _currentProject;
	
	bool _testLabSetup = false;
	bool _saved = false;

    cocos2d::DrawNode* _debugDrawNode;
	
	void _drawDot(cocos2d::Vec2 position);
    
    size_t _notificationListenerId;
	
};

}

#endif /* OPTestManager_hpp */
